#include "imgView.h"
#include "ui_imgView.h"

#include <QDebug>
#include <QKeyEvent>
#include "displayPolocy.h"
#include "imgMgr.h"
#include "fileMgr.h"

CImgView::CImgView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CImgView)
{
    ui->setupUi(this);
}

CImgView::~CImgView()
{
    delete ui;
}

int CImgView::init(const QString &imgPath)
{
    bool isSucc = false;
    do {
        //文件管理
        std::shared_ptr<CFileMgr> fileMgr(new CFileMgr);
        //TODO: 设置filter, order等
        if (0 != fileMgr->init(imgPath))
            break;

        //图片管理
        m_imgLoading.reset(new QPixmap(":/res/img/loading.jpg"));
        m_imgMgr = CImgMgrFac::create(fileMgr->size());
        if (0 != m_imgMgr->init(imgPath, fileMgr))
            break;
        connect(m_imgMgr.get(), &CImgMgr::sigLoaded, this, &CImgView::onSigLoaded);

        //显示策略
        m_displayPolicy.reset(new CRealSize);

        //显示图片
        auto pImgFile = m_imgMgr->cur();
        //TODO: 这段显示代码封函数
        qDebug() << "(display)-> " << pImgFile->m_info.absoluteFilePath();
        if (pImgFile->m_isReady)
            display(pImgFile->m_pImg.get());
        else
            display(m_imgLoading.get());

        isSucc = true;
    } while(0);

    if (!isSucc)
    {
        //释放资源
        m_imgMgr.reset();
        m_displayPolicy.reset();
    }

    return isSucc ? 0: -1;
}

int CImgView::displayPrev()
{
    auto pImgFile = m_imgMgr->prev();
    qDebug() << "<-(display) " << pImgFile->m_info.absoluteFilePath();
    if (pImgFile->m_isReady)
        display(pImgFile->m_pImg.get());
    else
        display(m_imgLoading.get());

    return 0;
}

int CImgView::displayNext()
{
    auto pImgFile = m_imgMgr->next();
    qDebug() << "(display)-> " << pImgFile->m_info.absoluteFilePath();
    if (pImgFile->m_isReady)
        display(pImgFile->m_pImg.get());
    else
        display(m_imgLoading.get());

    return 0;
}

void CImgView::onSigLoaded(QVariant var_file)
{
    if (!var_file.canConvert<QFileInfo>())
        return;

    auto file = var_file.value<QFileInfo>(); //TODO: 极端情况下, 仍然可能出现2个TImgFile所含file相同, 但pixmap*不同
    auto curFile = m_imgMgr->cur();
    if (curFile->m_info == file)
        display(curFile->m_pImg.get());
    return;
}

int CImgView::display(QPixmap *img)
{
    QPixmap scaled = *img;
    m_displayPolicy->process(&scaled);
    ui->lbImg->setPixmap(scaled);
    return 0;
}

void CImgView::keyPressEvent(QKeyEvent *ev)
{
    if (nullptr == ev)
        return;

    qDebug() << "-----" << ev;
    switch (ev->key())
    {
    case Qt::Key_Z:
        displayPrev();
        break;
    case Qt::Key_X:
        displayNext();
        break;
    default:
        break;
    }

    return;
}
