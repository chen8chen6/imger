#ifndef CIMGDSPAREA_H
#define CIMGDSPAREA_H

/*
    |<----------img.width----------->|
    ----------------------------------
    | dspArea外的图像对用户不可见      |
    |    |<------dsp.width------>|   |
    |    -------------------------   |
    |    | dspArea(填满屏幕)      |   |
    |    | 区域内图像对用户可见    |   |
    |    |                       |   |
    |    |       (中心焦点)       |   |
    |    |                       |   |
    |    |                       |   |
    |    |                       |   |
    |    -------------------------   |
    |                                |
    ----------------------------------
*/

#include <QScrollArea>
#include <memory>   //std::shared_ptr

class QLabel;
class QScrollBar;
class CDspStrategy;
namespace CFG
{
    class CCfgMgr;
    enum class DspStgy;
}

static constexpr int operator ""_percent(unsigned long long percent) { return percent; };

class CImgDspArea : public QScrollArea
{
    Q_OBJECT

public:
    CImgDspArea(QWidget* parent = nullptr);
    ~CImgDspArea() override {}
    int init(const CFG::CCfgMgr* cfgMgr);
    void setLbImg(QLabel* lb) { m_lbImg = lb; }
    int display(QPixmap* img);

protected:
    void keyPressEvent(QKeyEvent* ev) override;
    void wheelEvent(QWheelEvent* ev) override;
    void resizeEvent(QResizeEvent* ev) override;

private:
    static constexpr int ZOOM_MAX = 1000_percent;   //最大缩放倍率
    static constexpr int ZOOM_MIN = 5_percent;      //最小缩放倍率

private:
    //更新图片焦点
    void updateImgFocus(void);

    //显示区域左上角到中央的距离
    QPointF diff_topLeft2Center(void) const;

    //移动可见范围
    int moveSight(int dx, int dy);

    //缩放
    int zoom(int percent);  //percent = [ZOOM_MIN, ZOOM_MAX]
    QPixmap scaleImg(int percent) const;
    qreal zoomCoord(const qreal Xm, const int percent, const int curZoom,
        const int imgWidth, const int dspWidth) const;

    //dspArea的宽高(不包括边框和滚动条)
    int dspWidth(void) const;
    int dspHeight(void) const;

private:
    const CFG::CCfgMgr* m_cfgMgr = nullptr;
    QScrollBar* hScroll = nullptr;     //水平滚动条
    QScrollBar* vScroll = nullptr;     //垂直滚动条
    QLabel* m_lbImg = nullptr;      //用来显示图片的组件
    QPixmap m_curImg;       //原始图片

    //图片显示策略
    std::shared_ptr<CDspStrategy> m_dspStrategy = nullptr;

    struct {
        QPointF m_focus = QPointF(0.0, 0.0); //图片焦点, 即图片显示在dspArea正中的像素点坐标
        int m_zoom_percent = 100;       //缩放倍率, 100表示100%
    } m_dspSt;  //图片展示参数    //TODO: rename->imgCfg?
};

#endif // CIMGDSPAREA_H
