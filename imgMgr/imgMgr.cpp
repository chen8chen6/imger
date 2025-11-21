#include "imgMgr.h"
#include "cacheByNum.h"
#include "cacheAll.h"
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
    CImgLoader * imgLoader= new CImgLoader;
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

std::shared_ptr<CImgMgr> CImgMgrFac::create(int fileNum)
{
    if (fileNum > CCacheByNum::cacheSize())
        return std::shared_ptr<CImgMgr>(new CCacheByNum);//多数图片, 滚动方式缓存
    else
        return std::shared_ptr<CImgMgr>(new CCacheAll); //少量图片, 则缓存全部
}
