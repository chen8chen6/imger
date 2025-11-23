#include "imgDspArea.h"
#include <set>
#include <chrono>   //steady_clock::now()
#include <cmath>    //round()
#include <QDebug>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QLabel>
#include <QScrollBar>

using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;


CImgDspArea::CImgDspArea(QWidget *parent)
    : QScrollArea(parent)
{
    //m_lbImg = new QLabel(this);
    hScroll = horizontalScrollBar();
    vScroll = verticalScrollBar();
}

int CImgDspArea::display(QPixmap *img)
{
    m_curImg = *img;
    //TODO: dspPolicy.process
    m_lbImg->setPixmap(m_curImg);
    resetDspSt();
    return 0;
}

bool CImgDspArea::event(QEvent *ev)
{
    bool isProcessed = false;
    if (QEvent::KeyPress == ev->type())
    {
        auto keyEv = dynamic_cast<QKeyEvent*>(ev);
        //qDebug() << __FUNCTION__ << keyEv;

        switch (keyEv->key())
        {
        case Qt::Key_Plus:
            zoom(std::min(1000, m_dspSt.m_zoom_percent + 25));
            isProcessed = true;
            break;
        case Qt::Key_Minus:
            zoom(std::max(10, m_dspSt.m_zoom_percent - 25));
            isProcessed = true;
            break;
        case Qt::Key_Space:
            qDebug() << "focus:" << m_dspSt.m_focus <<", (h,v) =" << QPoint(hScroll->value(), vScroll->value());
            break;
        default:
            break;
        }
    }

    if (isProcessed)
        ev->accept();   //不再送到上层处理

    return isProcessed ? true : QScrollArea::event(ev);
}



void CImgDspArea::keyPressEvent(QKeyEvent *ev)
{
    //执行默认操作
    QScrollArea::keyPressEvent(ev);

    //方向键或翻页键
    std::set<int> arrowKeys { Qt::Key_Left, Qt::Key_Right, Qt::Key_Up,
                                Qt::Key_Down, Qt::Key_PageUp, Qt::Key_PageDown };
    const bool isArrowKey = (0 != arrowKeys.count(ev->key()));
    if (isArrowKey)
        updateImgFocus();    //更新图片焦点

    return;
}

void CImgDspArea::wheelEvent(QWheelEvent *ev)
{
    QScrollArea::wheelEvent(ev);    //执行默认操作
    updateImgFocus();    //更新图片焦点
    return;
}

void CImgDspArea::updateImgFocus()
{
    /**
      * 如果图片宽度<=显示区域宽度, 则scrollBar的[min, max]=[0, 0],
      * 此时焦点位于图片正中央, x坐标为 (img.width - 1)/2
      * 如果图片宽度>显示区域宽度, 则scrollBar的[min, max]=[0, img.width - scroll.pageStep],
      * 此时scroll.value即为图片位于显示区域最左上方的像素点坐标,
      **/

    QPoint topLeft(hScroll->value(), vScroll->value());
    m_dspSt.m_focus = topLeft + diff_topLeft2Center();
    //qDebug() << "top-left:" << topLeft << ", center:" << m_dspSt.m_focus;
    return;
}

QPointF CImgDspArea::diff_topLeft2Center() const
{
    /**
      * 已知显示区域的宽度 Wd = min(hScollBar.pageStep, img.width), 求中点x坐标 Xm.
      * 已知Wd每增大1个像素, Xm右移0.5个像素. 且有Xm(1)=0, Xm(2)=0.5, Xm(3)=1.
      * 即对任意Wd>1, 有 Xm(Wd) = Xm(Wd-1) + 0.5 = Xm(Wd-2) + 1 = ...,
      * 因此得通项公式为 Xm(Wd) = (Wd - 1)/2 (Wd>0)
      **/

    const auto imgSize = m_lbImg->pixmap()->size();
    const qreal w = std::min(dspWidth(), imgSize.width());
    const qreal h = std::min(dspHeight(), imgSize.height());
    return QPointF((w-1)/2.0, (h-1)/2.0);
}

void CImgDspArea::resetDspSt()
{
    m_dspSt.m_focus = QPoint(0, 0) + diff_topLeft2Center();
    m_dspSt.m_zoom_percent = 100;

    hScroll->setValue(0);
    vScroll->setValue(0);

    return;
}

int CImgDspArea::zoom(int percent)
{
    if (percent == m_dspSt.m_zoom_percent)
        return 0;

    //按比例缩放图片
    m_lbImg->setPixmap(scaleImg(percent));

    //TODO: 处理上溢出 和0,0,
    //if (m_dspSt.m_focus.x() > INT_MAX/ZOOM_MAX)

    //计算新焦点
    //TODO: 连按2次放大时, 如果 图片处理速度较慢, 会造成焦点未及时按定位调整
    //TODO: 缓存一张图片的多种倍率
    int curZoom = m_dspSt.m_zoom_percent;
    const QSize scaled = m_lbImg->pixmap()->size();
    double xm2 = zoomCoord(m_dspSt.m_focus.x(), percent, curZoom, scaled.width(), dspWidth());
    double ym2 = zoomCoord(m_dspSt.m_focus.y(), percent, curZoom, scaled.height(), dspHeight());
    QPointF newFocus(xm2, ym2);

    //计算scrollBar的value, 即图片显示在dspArea最左上方的像素点的坐标
    QPointF newTopLeft = newFocus - diff_topLeft2Center();   //TODO: <0?
    qDebug() << "focus:" << m_dspSt.m_focus << "->" << newFocus;
    qDebug() << "O:" << newTopLeft;

    //更新展示状态
    m_dspSt.m_focus = newFocus;
    m_dspSt.m_zoom_percent = percent;

    //TODO: scrollWigetCont部件尺寸变化后, 会触发一个resize事件, scrollBar.max那时才会自动更新, 焦点定位在那里做
    int hsMax = scaled.width() < dspWidth() ? 0 : scaled.width() - dspWidth();
    int vsMax = scaled.height() < dspHeight() ? 0 : scaled.height() - dspHeight();
    hScroll->setMaximum(hsMax);
    vScroll->setMaximum(vsMax);
    hScroll->setValue(static_cast<int>(round(newTopLeft.x())));
    vScroll->setValue(static_cast<int>(round(newTopLeft.y())));

    return 0;
}

QPixmap CImgDspArea::scaleImg(int percent) const
{
    //缩放当前图片
    auto start = steady_clock::now();
    int curZoom = m_dspSt.m_zoom_percent;
    QPixmap scaled = m_curImg.scaled(m_curImg.size()*percent/100,
                                     Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation);
    auto cost = duration_cast<milliseconds>(steady_clock::now()-start);

    qDebug() << "zoom(" << cost.count() << "ms)" << curZoom << "->" << percent;
    qDebug() << " size:" << m_lbImg->pixmap()->size() << "->" << scaled.size();
    return scaled;
}

qreal CImgDspArea::zoomCoord(const qreal Xm, const int percent, const int curZoom, const int imgWidth, const int dspWidth) const
{
    /**
     * 设中心点横坐标为 Xm, 则有图片可见横坐标范围为[Xm - L, Xm + L] = [S1, E1] (L*2 + 1 = dsp.width)
     * Xm本身宽度1像素, 其中心离原点的距离为 Xm + 0.5,
     * 将图片缩放 z(z>0) 倍后, Xm2离原点距离为 Xm2 + 0.5 = (Xm + 0.5) * z,
     * 可得通项公式: Xm2 = (Xm - 0.5) * z + 0.5.
     * 新的可见横坐标范围为[Xm2 - L, Xm2 + L] = [S2, E2], 需落在图片范围内
     * 故有:
     * f(x) = (x + 0.5) * z - 0.5
     * Xm2 = (img.width - 1) / 2    (S2 < 0 && imgZ.width <= E2)    //可见范围大于图片宽度, 焦点为图片中央
     * Xm2 = f(Xm) + |S2|           (S2 < 0 && E2 < imgZ.width)     //可见范围超出图片左侧, 整体向右移动|S2|距离
     * Xm2 = f(Xm)                  (0 <= S2 && E2 < imgZ.width)    //可见范围未超出图片
     * Xm2 = f(Xm) - |E2 - imgZ.w|  (0 <= S2 && imgZ.width <= E2)   //可见范围超出图片右侧, 整体向左移动|E2 - imgZ.width| 距离
     */

    if (imgWidth <= dspWidth)
        return (imgWidth - 1.0) / 2.0;

    qreal len = (dspWidth - 1.0) / 2.0;
    qreal Xm2 = (Xm + 0.5) * percent / curZoom - 0.5;
    qreal s2 = Xm2 - len;
    qreal e2 = Xm2 + len;
    qreal moveRight = s2 < 0 ? std::abs(s2) : 0.0;
    qreal moveLeft = (imgWidth <= e2) ? (e2 - imgWidth) : 0.0;
    Xm2 = Xm2 + moveRight - moveLeft;
    return Xm2;
}

int CImgDspArea::dspWidth() const
{
    return hScroll->pageStep();
}

int CImgDspArea::dspHeight() const
{
    return vScroll->pageStep();
}

