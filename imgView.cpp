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
        m_cache = new CImgCache;
        if (0 != m_cache->init(imgPath))
            break;

        //显示策略
        m_displayPolicy = new CRealSize;

        //显示图片
        auto imgFile = m_cache->cur();
        if (nullptr == imgFile.second
                //|| imgFile.second->isNull()
                )
        {
            qDebug() << "invalid img: " << imgFile.first.absoluteFilePath();
            break;
        }

        m_curImg = imgFile.second;
        if (0 != display(m_curImg))
        {
            qDebug() << "display img faild: " << imgFile.first.absoluteFilePath();
            break;
        }

        isSucc = true;
    } while(0);

    if (!isSucc)
    {
        //释放资源
        if (nullptr != m_cache)
        {
            delete m_cache;
            m_cache = nullptr;
        }

        if (nullptr != m_displayPolicy)
        {
            delete m_displayPolicy;
            m_displayPolicy = nullptr;
        }
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
