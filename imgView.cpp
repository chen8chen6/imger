#include "imgView.h"
#include "ui_imgView.h"

#include <QDebug>
#include "imgCache.h"
#include "displayPolocy.h"

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
        //图片缓冲
        m_cache.reset(new CImgCache);
        if (0 != m_cache->init(imgPath))
            break;

        //显示策略
        m_displayPolicy.reset(new CRealSize);

        //显示图片
        auto imgFile = m_cache->cur();
        if (nullptr == imgFile.m_pImg
                || imgFile.m_pImg->isNull())
        {
            qDebug() << "invalid img: " << imgFile.m_info.absoluteFilePath();
            break;
        }

        m_curImg = imgFile.m_pImg;
        if (0 != display(m_curImg.get()))
        {
            qDebug() << "display img faild: " << imgFile.m_info.absoluteFilePath();
            break;
        }

        isSucc = true;
    } while(0);

    if (!isSucc)
    {
        //释放资源
        m_cache.reset();
        m_displayPolicy.reset();
        m_curImg.reset();
    }

    return isSucc ? 0: -1;
}

int CImgView::display(QPixmap *img)
{
    QPixmap scaled = *img;
    m_displayPolicy->process(&scaled);
    ui->lbImg->setPixmap(scaled);
    return 0;
}
