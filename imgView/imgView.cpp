#include "imgView.h"
#include "ui_imgView.h"

#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
#include <QMessageBox>
#include "imgMgr.h"
#include "fileMgr.h"
#include "cfgMgr.h"

using CFG::CCfgMgr;
using CFG::CCfgHelper;
using CFG::keyHash_t;
using CFG::Usage;

CImgView::CImgView(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::CImgView)
{
    ui->setupUi(this);

    ui->scrollArea->setLbImg(ui->lbImg);

    //图片载入完成时, 进行更新
    m_checkLoaded = new QTimer(this);
    connect(m_checkLoaded, &QTimer::timeout, this, &CImgView::updateIfLoaded);
}

CImgView::~CImgView()
{
    delete ui;
}

int CImgView::init(const QString& filePath, const CCfgMgr* cfgMgr)
{
    bool isSucc = false;
    do {
        //载入设置
        if (nullptr == cfgMgr)
            break;
        m_cfgMgr = cfgMgr;
        auto cfg = m_cfgMgr->getCfg();

        //图片管理
        //如果返回空指针,代表初始化失败
        auto dspOrder = (static_cast<int>(CFG::DspOrder::ByName) == cfg->imgView.dspOrder)    //TODO: 在imgMgr类的内部实际处理最终顺序
            ? QDir::SortFlags(QDir::Name | QDir::IgnoreCase)
            : QDir::SortFlags(QDir::Time);
        m_imgMgr = CImgMgrFac::create(filePath, dspOrder);
        if (nullptr == m_imgMgr)
            break;

        //设置显示策略
        ui->scrollArea->init(cfgMgr);

        //显示图片
        display(m_imgMgr->cur().get());

        isSucc = true;
    } while (0);

    if (!isSucc)
    {
        //释放资源
        m_imgMgr.reset();
    }

    return isSucc ? 0 : -1;
}

int CImgView::displayPrev()
{
    return display(m_imgMgr->prev().get());
}

int CImgView::displayNext()
{
    return display(m_imgMgr->next().get());
}

int CImgView::display(QPixmap* img)
{
    QPixmap scaled = *img;
    ui->scrollArea->display(&scaled);
    return 0;
}

int CImgView::display(tag_imgFile* pImgFile)
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

void CImgView::keyPressEvent(QKeyEvent* ev)
{
    if (nullptr == ev)
        return;

    qDebug() << "imgView:" << ev;

    keyHash_t keyHash = CCfgHelper::getKeyHash(ev);
    const auto usageDict = m_cfgMgr->getCfg()->imgView.keyUsageDict;
    if (0 == usageDict.count(keyHash))
        return;

    switch (usageDict.at(keyHash))
    {
    case Usage::Help:
        QMessageBox::information(this, "F1", "Help here");
        break;
    case Usage::Next_Img:
        displayNext();
        break;
    case Usage::Prev_Img:
        displayPrev();
        break;
    default:
        //do Nothing
        break;
    }

    ev->accept();
    return;
}

void CImgView::updateIfLoaded()
{
    auto pImgFile = m_imgMgr->cur();
    if (m_imgMgr->cur()->m_isReady)
        display(pImgFile.get());    //缓存完毕, 更新图片
}
