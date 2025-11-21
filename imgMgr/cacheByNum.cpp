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
        QFileInfoList file2Cache = m_fileMgr->nFilesAfter(FORWARD_CACHE_SIZE, curImg);
        for (auto file : file2Cache)
            cacheNext(file);

        //向前缓存
        file2Cache = m_fileMgr->nFilesBefore(BACKWARD_CACHE_SIZE, curImg);
        for (auto file : file2Cache)
            cachePrev(file);

        qDebug() << "=== cache ===" ;
        for (const auto &imgFile : m_cache)
            qDebug() << imgFile->m_info.fileName();
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
    qDebug() << "<-(cache): " << m_cache.back()->m_info.fileName();
    m_cache.pop_back(); //向后缓冲区-1
    auto file2Cache = m_fileMgr->fileBefore(m_cache.front()->m_info);
    qDebug() << "(cache)<-: " << file2Cache.fileName();
    cachePrev(file2Cache);  //向前缓冲区+1
    --m_cur;

    return *m_cur;
}

pImgFile_t CCacheByNum::next()
{
    //维护缓冲区
    qDebug() << "<-(cache): " << &m_cache.front() << m_cache.front()->m_info.fileName();
    m_cache.pop_front(); //向前缓冲区-1
    auto file2Cache = m_fileMgr->fileAfter(m_cache.back()->m_info);
    qDebug() << "(cache)<-: " << file2Cache.fileName();
    cacheNext(file2Cache);  //向后缓冲区+1
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
