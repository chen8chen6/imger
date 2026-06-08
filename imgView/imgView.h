#ifndef IMGVIEW_H
#define IMGVIEW_H

#include <QDialog>
#include <memory>   //std::shared_ptr
#include "imgType.h"    //pImgFile_t

namespace Ui {
    class CImgView;
}

namespace CFG
{
    class CCfgMgr;
}

class CImgMgr;
struct tag_imgFile;

class CImgView : public QDialog
{
    Q_OBJECT

public:
    explicit CImgView(QWidget* parent = nullptr);
    ~CImgView() override;
    int init(const QString& filePath, const CFG::CCfgMgr* cfgMgr);

    //显示图片
    int displayPrev(void);
    int displayNext(void);

private slots:
    void  updateIfLoaded(void);

private:
    //显示
    int display(const pImgFile_t& pImgFile);

    //键盘事件响应
    void keyPressEvent(QKeyEvent* ev) override;

private:
    static constexpr int INTERVAL_CHECK_IMG_LOADED_MS = 50; //检查图像是否载入完成的时间间隔

    Ui::CImgView* ui;

    QTimer* m_checkLoaded = nullptr;   //每隔一段时间检查图片是否加载完成
    std::shared_ptr<CImgMgr> m_imgMgr = nullptr;                //图片缓存池

    const CFG::CCfgMgr* m_cfgMgr = nullptr;
};

#endif // IMGVIEW_H
