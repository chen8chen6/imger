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

        //图片缓冲
        m_imgMgr = CImgMgrFac::create(fileMgr->size());
        if (0 != m_imgMgr->init(imgPath, fileMgr))
            break;

        //显示策略
        m_displayPolicy.reset(new CRealSize);

        //显示图片
        auto imgFile = m_imgMgr->cur();
        if (nullptr == imgFile.m_pImg
                || imgFile.m_pImg->isNull())
        {
            qDebug() << "invalid img: " << imgFile.m_info.absoluteFilePath();
            break;
        }

        if (0 != display(imgFile.m_pImg.get()))
        {
            qDebug() << "display img faild: " << imgFile.m_info.absoluteFilePath();
            break;
        }

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
    auto imgFile = m_imgMgr->prev();
    qDebug() << "<-(display) " << imgFile.m_info.absoluteFilePath();
    std::unique_lock<std::mutex> locker(*(imgFile.m_mutex));
    display(imgFile.m_pImg.get());
    return 0;
}

int CImgView::displayNext()
{
    auto imgFile = m_imgMgr->next();
    qDebug() << "(display)-> " << imgFile.m_info.absoluteFilePath();
    std::unique_lock<std::mutex> locker(*(imgFile.m_mutex));
    display(imgFile.m_pImg.get());
    return 0;
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
