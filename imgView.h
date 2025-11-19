#ifndef IMGVIEW_H
#define IMGVIEW_H

#include <QDialog>

namespace Ui {
class CImgView;
}

//class QPixmap;
class CImgCache;
class CDisplayPolicy;

class CImgView : public QDialog
{
    Q_OBJECT

public:
    explicit CImgView(QWidget *parent = nullptr);
    ~CImgView() override;
    int init(const QString &imgPath);

    //显示图片
    int displayPrev(void);
    int displayNext(void);

private:
    //显示
    int display(QPixmap *img);

    //移动
    int moveVisableArea(const int dx, const int dy);

    //缩放
    int zoom(const int percent);

private:
    Ui::CImgView *ui;

    QPixmap *m_curImg = nullptr;
    CImgCache *m_cache = nullptr;               //图片缓存池
    CDisplayPolicy *m_displayPolicy = nullptr;  //图片显示状态
    struct {
        int m_centerX = 0;          //当前焦点
        int m_centerY = 0;
        int m_zoomPercent = 100;    //当前缩放倍率
    } m_curSt;                  //显示状态

};

#endif // IMGVIEW_H
