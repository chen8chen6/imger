#ifndef CIMGDSPAREA_H
#define CIMGDSPAREA_H

#if 0
|<----------img.width----------->|
----------------------------------
|dspArea外的图像对用户不可见        |
|    |<------dsp.width------>|   |
|    -------------------------   |
|    | dspArea(填满屏幕)      |   |
|    | 区域内图像对用户可见     |   |
|    |                       |   |
|    |       (中心焦点)       |   |
|    |                       |   |
|    |                       |   |
|    |                       |   |
|    -------------------------   |
|                                |
----------------------------------
#endif

#include <QScrollArea>
#include <memory>   //std::shared_ptr

class QLabel;
class QScrollBar;
class CDspStrategy;
class CImgDspArea : public QScrollArea
{
    Q_OBJECT

public:
    enum class DSP_STRATEGY {RealSize = 0, FitWin,};    //图片显示策略

public:
    CImgDspArea(QWidget *parent = nullptr);
    ~CImgDspArea() override {}

    void setLbImg(QLabel *lb) {m_lbImg = lb;}
    void setDspStrategy(const DSP_STRATEGY strategy);
    int display(QPixmap *img);

protected:
    bool event(QEvent *ev) override;
    //bool eventFilter(QObject *sender, QEvent *ev) override;

    void keyPressEvent(QKeyEvent *ev) override;
    void wheelEvent(QWheelEvent *ev) override;

    void resizeEvent(QResizeEvent *ev) override;

private:
    //更新图片焦点
    void updateImgFocus(void);

    //显示区域左上角到中央的距离
    QPointF diff_topLeft2Center(void) const;

    //缩放
    int zoom(int percent);  //percent = [ZOOM_MIN, ZOOM_MAX]
    QPixmap scaleImg(int percent) const;
    qreal zoomCoord(const qreal Xm, const int percent, const int curZoom,
                    const int imgWidth, const int dspWidth) const;

    //dspArea的宽高(不包括边框和滚动条)
    int dspWidth(void) const;
    int dspHeight(void) const;

private:
    static constexpr int ZOOM_MAX = 1000;   //最大缩放倍率
    static constexpr int ZOOM_MIN = 25;     //最小缩放倍率

    QScrollBar * hScroll = nullptr;     //水平滚动条
    QScrollBar * vScroll = nullptr;     //垂直滚动条
    QLabel *m_lbImg = nullptr;      //用来显示图片的组件
    QPixmap m_curImg;       //原始图片

    //图片显示策略
    std::shared_ptr<CDspStrategy> m_dspStrategy = nullptr;
    DSP_STRATEGY m_curDspStrategy = DSP_STRATEGY::RealSize;

    struct {
        QPointF m_focus = QPointF(0.0, 0.0); //图片焦点, 即图片显示在dspArea正中的像素点坐标
        int m_zoom_percent = 100;       //缩放倍率, 100表示100%
    } m_dspSt;  //图片展示参数    //TODO: rename->imgCfg?
};

#endif // CIMGDSPAREA_H
