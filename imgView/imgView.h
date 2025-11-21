#ifndef IMGVIEW_H
#define IMGVIEW_H

#include <QDialog>
#include <memory>   //std::shared_ptr

namespace Ui {
class CImgView;
}

//class QPixmap;
class CImgMgr;
class CDisplayPolicy;
struct tag_imgFile;

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
    int display(tag_imgFile * pImgFile);

    //移动
    int moveVisableArea(const int dx, const int dy);

    //缩放
    int zoom(const int percent);

    //键盘事件响应
    void keyPressEvent(QKeyEvent *ev) override;

private slots:
    void  updateIfLoaded(void);

private:
    static constexpr int INTERVAL_CHECK_IMG_LOADED_MS = 50; //检查图像是否载入完成的时间间隔

    Ui::CImgView *ui;

    QTimer * m_checkLoaded = nullptr;   //每隔一段时间检查图片是否加载完成
    std::shared_ptr<CImgMgr> m_imgMgr = nullptr;                //图片缓存池
    std::shared_ptr<CDisplayPolicy> m_displayPolicy = nullptr;  //图片显示状态
    struct {
        int m_centerX = 0;          //当前焦点
        int m_centerY = 0;
        int m_zoomPercent = 100;    //当前缩放倍率
    } m_curSt;                  //显示状态

};

#endif // IMGVIEW_H
