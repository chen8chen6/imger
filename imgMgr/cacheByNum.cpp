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
        m_cache = {{curImg, pImg_t(new QPixmap(imgPath)), std::shared_ptr<std::mutex>(new std::mutex)}};
        m_cur = m_cache.begin();

        QFileInfoList file2Cache = m_fileMgr->nFilesAfter(FORWARD_CACHE_SIZE, curImg);
        for (auto file : file2Cache)
            cacheNext(file);

        file2Cache = m_fileMgr->nFilesBefore(BACKWARD_CACHE_SIZE, curImg);
        for (auto file : file2Cache)
            cachePrev(file);

        qDebug() << "=== cache ===" ;
        for (const auto &imgFile : m_cache)
            qDebug() << imgFile.m_info.fileName() << imgFile.m_pImg->size();
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

TImgFile CCacheByNum::prev()
{
    //维护缓冲区
    qDebug() << "<-(cache): " << m_cache.rbegin()->m_info.fileName();
    m_cache.pop_back(); //向后缓冲区-1
    auto file2Cache = m_fileMgr->fileBefore(m_cache.begin()->m_info);
    qDebug() << "(cache)<-: " << file2Cache.fileName();
    cachePrev(file2Cache);  //向前缓冲区+1
    --m_cur;

    return *m_cur;
}

TImgFile CCacheByNum::next()
{
    //维护缓冲区
    qDebug() << "<-(cache): " << m_cache.begin()->m_info.fileName();
    m_cache.pop_front(); //向前缓冲区-1
    auto file2Cache = m_fileMgr->fileAfter(m_cache.rbegin()->m_info);
    qDebug() << "(cache)<-: " << file2Cache.fileName();
    cacheNext(file2Cache);  //向后缓冲区+1
    ++m_cur;

    return *m_cur;
}

int CCacheByNum::cachePrev(QFileInfo file)
{
    //m_cache.push_front({file, pImg_t(new QPixmap(file.absoluteFilePath()))});
    TImgFile imgFile{file, pImg_t(new QPixmap), std::shared_ptr<std::mutex>(new std::mutex)};
    m_cache.push_front(imgFile);
    emit sigLoadImg(QVariant::fromValue(imgFile));
    return 0;
}

int CCacheByNum::cacheNext(QFileInfo file)
{
    //m_cache.push_back({file, pImg_t(new QPixmap(file.absoluteFilePath()))});
    TImgFile imgFile{file, pImg_t(new QPixmap), std::shared_ptr<std::mutex>(new std::mutex)};
    m_cache.push_back(imgFile);
    emit sigLoadImg(QVariant::fromValue(imgFile));
    return 0;
}
