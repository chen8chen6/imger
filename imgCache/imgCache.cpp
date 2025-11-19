#include "imgCache.h"
#include <algorithm> //std::min
#include <QDebug>
#include "fileMgr.h"
#include "imgCacheBuffer.h"

CImgCache::CImgCache()
{

}

CImgCache::~CImgCache()
{

}

int CImgCache::init(const QString &imgPath)
{
    bool isSucc = false;

    do
    {
        //文件列表
        m_fileMgr.reset(new CFileMgr);
        if (0 != m_fileMgr->init(imgPath))
            break;

        //缓冲
        qDebug() << "=== cache ===";
        m_buf.reset(new CImgCacheBuffer);
        constexpr int bufSize = CImgCacheBuffer::BUF_SIZE;
        const int cacheCnt = std::min(m_fileMgr->size(), bufSize);

        //TODO: 简化缓冲流程
        //缓冲第一张
        m_buf->cache(QFileInfo(imgPath), QPixmap(imgPath));

        //缓冲剩余的图片
        for (int i = 1; i < cacheCnt; ++i)
        {
            auto file2Cache = m_fileMgr->next();
            m_buf->cache(file2Cache, QPixmap(file2Cache.absoluteFilePath()));
        }
        qDebug() << "=============";

        isSucc = true;
    } while(0);

    if (!isSucc)
    {
        //释放资源
        m_fileMgr.reset();
        m_buf.reset();
    }

    return isSucc ? 0 : -1;
}

TImgFile CImgCache::cur() const
{
    return m_buf->cur();
}

TImgFile CImgCache::next()
{
    //TODO: 这段代码可读性差
    auto ret = m_buf->next();

    //更新缓冲区
    QFileInfo file2Cache = m_fileMgr->next();
    m_buf->cache(file2Cache, QPixmap(file2Cache.absoluteFilePath()));

    return ret;
}

