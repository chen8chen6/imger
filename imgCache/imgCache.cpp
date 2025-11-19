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
    if (nullptr != m_fileMgr)
        delete m_fileMgr;
    if (nullptr != m_buf)
        delete m_buf;
    return;
}

int CImgCache::init(const QString &imgPath)
{
    bool isSucc = false;

    do
    {
        //文件列表
        m_fileMgr = new CFileMgr;
        if (0 != m_fileMgr->init(imgPath))
            break;
        m_curFile = QFileInfo(imgPath);

        //缓冲
        qDebug() << "=== cache ===";
        m_buf = new CImgCacheBuffer;
        constexpr int bufSize = CImgCacheBuffer::BUF_SIZE;
        const int cacheCnt = std::min(m_fileMgr->size(), bufSize);
        for (int i = 0; i < cacheCnt; ++i)
        {
            const QString imgPath = m_fileMgr->next().absoluteFilePath();
            qDebug() << imgPath;
            QPixmap img(imgPath);
            qDebug() << img.size();
            m_buf->cache(img);
        }
        qDebug() << "=============";

        isSucc = true;
    } while(0);

    if (!isSucc)
    {
        //释放资源
        if (nullptr != m_fileMgr)
        {
            delete m_fileMgr;
            m_fileMgr = nullptr;
        }

        if (nullptr != m_buf)
        {
            delete m_buf;
            m_buf = nullptr;
        }
    }

    return isSucc ? 0 : -1;
}

CImgCache::imgFile_t CImgCache::cur()
{
    return {m_curFile, m_buf->first()};
}

