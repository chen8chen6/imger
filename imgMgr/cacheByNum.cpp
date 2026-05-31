#include "cacheByNum.h"

#include <QPixmap>
#include <QDebug>
#include "fileMgr.h"

CCacheByNum::CCacheByNum()
{
    initImgLoaderThr();
}

CCacheByNum::~CCacheByNum()
{
    uninitImgLoaderThr();
}

int CCacheByNum::init(const QString &imgPath, std::shared_ptr<CFileMgr> fileMgr)
{
    bool isSucc = false;
    do
    {
        qDebug() << name();

        //文件管理
        m_fileMgr = fileMgr;
        QFileInfo curImg(imgPath);
        if (!m_fileMgr->exist(curImg))
            break;  //不是图片文件

        //缓存
        m_cache.clear();
        cacheNext(curImg);
        m_cur = m_cache.begin();

        //向后缓存
        //预计行为是向后浏览, 因此往这个反向多缓冲一些图片
        //TODO: 调整缓冲顺序, 避免向前浏览时需要等待大量图片向后缓冲结束
        QFileInfoList file2Cache = m_fileMgr->nFilesAfter(NEXT_CACHE_MOST, curImg);
        for (auto file : file2Cache)
            cacheNext(file);
        m_nextCached = NEXT_CACHE_MOST;

        //向前缓存
        file2Cache = m_fileMgr->nFilesBefore(PREV_CACHE_AVG, curImg);
        for (auto file : file2Cache)
            cachePrev(file);
        m_prevCached = PREV_CACHE_AVG;

        qDebug() << "=== cache ===" ;
        for (const auto &imgFile : m_cache)
            qDebug() << imgFile->m_info.fileName();
        qDebug() << "cached prev:" << m_prevCached << ", next:" << m_nextCached;
        qDebug() << "=============";

        isSucc = true;
    } while(0);

    if (!isSucc)
    {
        m_fileMgr.reset();
        m_cache.clear();
    }

    return isSucc ? 0 : -1;
}

pImgFile_t CCacheByNum::prev()
{
    //维护缓冲区
    //向后缓存至多next_most个元素, 高于该水平则删减到next_lv个元素.
    if (++m_nextCached > NEXT_CACHE_MOST)
    {
        for (int i = 0; i < m_nextCached - NEXT_CACHE_AVG; ++i)
        {
            qDebug() << "<-(cache): " << m_cache.back()->m_info.fileName();
            m_cache.pop_back();
        }
        m_nextCached = NEXT_CACHE_AVG;
    }

    //向前缓存至少prev_least个元素, 低于该水平则补充到prev_lv个元素.
    if (--m_prevCached < PREV_CACHE_LEAST)
    {
        auto fileList = m_fileMgr->nFilesBefore(PREV_CACHE_AVG - m_prevCached, m_cache.front()->m_info);
        for (auto &file2Cache : fileList)
        {
            qDebug() << "(cache)<-: " << file2Cache.fileName();
            cachePrev(file2Cache);
        }
        m_prevCached = PREV_CACHE_AVG;
    }
    qDebug() << "cached prev:" << m_prevCached << ", next:" << m_nextCached;
    --m_cur;
    return *m_cur;
}

pImgFile_t CCacheByNum::next()
{
    //维护缓冲区
    //向前缓存至多prev_most个元素, 高于该水平则删减到prev_lv个元素.
    if (++m_prevCached > PREV_CACHE_MOST)
    {
        for (int i = 0; i < m_prevCached - PREV_CACHE_AVG; ++i)
        {
            qDebug() << "<-(cache): " << m_cache.front()->m_info.fileName();
            m_cache.pop_front();
        }
        m_prevCached = PREV_CACHE_AVG;
    }

    //向后缓存至少next_least个元素, 低于该水平则补充到next_lv个元素.
    if (--m_nextCached < NEXT_CACHE_LEAST)
    {
        auto fileList = m_fileMgr->nFilesAfter(NEXT_CACHE_AVG - m_nextCached, m_cache.back()->m_info);
        for (auto& file2Cache : fileList)
        {
            qDebug() << "(cache)<-: " << file2Cache.fileName();
            cacheNext(file2Cache);
        }
        m_nextCached = NEXT_CACHE_AVG;
    }
    qDebug() << "cached prev:" << m_prevCached << ", next:" << m_nextCached;

    ++m_cur;
    return *m_cur;
}

int CCacheByNum::cachePrev(QFileInfo file)
{
    pImgFile_t pImgFile(new TImgFile(file, pImg_t(new QPixmap)));
    m_cache.push_front(pImgFile);
    emit sigLoadImg(QVariant::fromValue(m_cache.front()));
    return 0;
}

int CCacheByNum::cacheNext(QFileInfo file)
{
    pImgFile_t pImgFile(new TImgFile(file, pImg_t(new QPixmap)));
    m_cache.push_back(pImgFile);
    emit sigLoadImg(QVariant::fromValue(m_cache.back()));
    return 0;
}
