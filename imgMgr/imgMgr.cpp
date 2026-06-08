#include "imgMgr.h"
#include <QImageReader> //supportedImageFormats()
#include "cacheByNum.h"
#include "cacheAll.h"
#include "fileMgr.h"
#include "imgLoader.h"

CImgMgr::CImgMgr()
{

}

CImgMgr::~CImgMgr()
{

}

int CImgMgr::initImgLoaderThr()
{
    //读图线程
    CImgLoader* imgLoader = new CImgLoader;
    imgLoader->moveToThread(&m_imgLoaderThr);
    connect(&m_imgLoaderThr, &QThread::finished, imgLoader, &QObject::deleteLater);
    connect(this, &CImgMgr::sigLoadImg, imgLoader, &CImgLoader::onSigLoadImg);
    m_imgLoaderThr.start();
    return 0;
}

int CImgMgr::uninitImgLoaderThr()
{
    m_imgLoaderThr.quit();
    m_imgLoaderThr.wait();
    return 0;
}

std::shared_ptr<CImgMgr> CImgMgrFac::create(const QString& filePath, QDir::SortFlags sorting)
{
    //获取支持的图片类型的后缀名
    const auto supportImageFormats = QImageReader::supportedImageFormats();
    QStringList imgSuffix;
    imgSuffix.reserve(supportImageFormats.size());
    for (const auto& fmt : supportImageFormats)
        imgSuffix << QStringLiteral("*.%1").arg(QString(fmt));

    std::shared_ptr<CImgMgr> ret = nullptr;
    bool isSucc = false;

    do {
        QFileInfo file(filePath);
        if (!file.exists())
            break;  //文件不存在

        //初始化文件管理
        QDir dir(file.absolutePath());
        dir.setNameFilters(imgSuffix);
        dir.setFilter(QDir::Filters(QDir::Files));
        dir.setSorting(sorting);

        std::shared_ptr<CFileMgr> fileMgr(new CFileMgr);
        if (0 != fileMgr->init(filePath, dir))
            break;  //fileMgr初始化失败

        //挑选合适的imgMgr
        ret = (fileMgr->size() < CCacheByNum::cacheSize())
            ? std::shared_ptr<CImgMgr>(new CCacheAll)   //缓存所有图片
            : std::shared_ptr<CImgMgr>(new CCacheByNum);//按数量缓存图片

        //初始化图片管理
        if (0 != ret->init(filePath, fileMgr))
            break;  //imgMgr初始化失败

        isSucc = true;
    } while (0);

    return isSucc ? ret : nullptr;
}
