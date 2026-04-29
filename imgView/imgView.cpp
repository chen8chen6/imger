#include "imgView.h"
#include "ui_imgView.h"

#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
#include "imgMgr.h"
#include "fileMgr.h"

CImgView::CImgView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CImgView)
{
    ui->setupUi(this);

    ui->scrollArea->setLbImg(ui->lbImg);
    ui->scrollArea->setDspStrategy(CImgDspArea::DSP_STRATEGY::RealSize);

    //图片载入完成时, 进行更新
    m_checkLoaded = new QTimer(this);
    connect(m_checkLoaded, &QTimer::timeout, this, &CImgView::updateIfLoaded);
}

CImgView::~CImgView()
{
    delete ui;
}

int CImgView::init(const QString &filePath)
{
    bool isSucc = false;
    do {
        //图片管理
        //如果返回空指针,代表初始化失败
        m_imgMgr = CImgMgrFac::create(filePath, QDir::SortFlags(QDir::Name | QDir::IgnoreCase));
        if (nullptr == m_imgMgr)
            break;

        //设置显示策略
        ui->scrollArea->setDspStrategy(CImgDspArea::DSP_STRATEGY::FitWin);

        //显示图片
        display(m_imgMgr->cur().get());

        isSucc = true;
    } while(0);

    if (!isSucc)
    {
        //释放资源
        m_imgMgr.reset();
    }

    return isSucc ? 0: -1;
}

int CImgView::displayPrev()
{
    return display(m_imgMgr->prev().get());
}

int CImgView::displayNext()
{
    return display(m_imgMgr->next().get());
}

int CImgView::display(QPixmap *img)
{
    QPixmap scaled = *img;
    ui->scrollArea->display(&scaled);
    return 0;
}

int CImgView::display(tag_imgFile *pImgFile)
{
    qDebug() << "(display)-> " << pImgFile->m_info.absoluteFilePath();
    if (pImgFile->m_isReady)
    {
        display(pImgFile->m_pImg.get());
        m_checkLoaded->stop();
    }
    else
    {
        static QPixmap loadingImg(":/res/img/loading.jpg");
        display(&loadingImg);

        //设置定时器, 每隔一段时间查看图片加载完成
        if (!m_checkLoaded->isActive())
            m_checkLoaded->start(INTERVAL_CHECK_IMG_LOADED_MS);
    }
    return 0;
}

void CImgView::keyPressEvent(QKeyEvent *ev)
{
    if (nullptr == ev)
        return;

    qDebug() << "imgView:" << ev;
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

void CImgView:: updateIfLoaded()
{
    auto pImgFile = m_imgMgr->cur();
    if (m_imgMgr->cur()->m_isReady)
        display(pImgFile.get());    //缓存完毕, 更新图片
}
