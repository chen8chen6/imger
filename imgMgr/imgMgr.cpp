#include "imgMgr.h"
#include <QPixmap>
#include <QDebug>
#include "fileMgr.h"

CImgMgr::CImgMgr()
{

}

int CImgMgr::init(const QString &imgPath)
{
    bool isSucc = false;
    do
    {
        //文件管理
        m_fileMgr.reset(new CFileMgr);
        if (0 != m_fileMgr->init(imgPath))
            break;

        QFileInfo curImg(imgPath);
        if (!m_fileMgr->exist(curImg))
            break;  //不是图片文件

        if (FORWARD_CACHE_SIZE + BACKWARD_CACHE_SIZE + 1 < 3)
            break;  //缓冲区太小, 处理起来费劲

        //缓存
        //TODO: 处理缓冲区比fileList大的情况
        m_cache = {{curImg, pImg_t(new QPixmap(imgPath))}};
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

TImgFile CImgMgr::prev()
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

TImgFile CImgMgr::next()
{
    //列表中只有1个文件
    if (1 == m_cache.size())
        return *m_cur;

    //整个文件夹都已经进了缓存区, 缓存区不需要后续维护
    //TODO: 由imgView先初始化fileMgr, 然后根据list.size选择不同的imgMgr实现
    if (isWholeDirCached())
    {
        if (m_cache.end() == ++m_cur)
            m_cur = m_cache.begin();
        return *m_cur;
    }

    //维护缓冲区
    //m_cur前后必定有有效元素, 因为cache.size > 3, 且m_cur位于中间
    qDebug() << "<-(cache): " << m_cache.begin()->m_info.fileName();
    m_cache.pop_front(); //向前缓冲区-1
    auto file2Cache = m_fileMgr->fileAfter(m_cache.rbegin()->m_info);
    qDebug() << "(cache)<-: " << file2Cache.fileName();
    cacheNext(file2Cache);  //向后缓冲区+1
    ++m_cur;

    return *m_cur;
}

int CImgMgr::cachePrev(QFileInfo file)
{
    m_cache.push_front({file, pImg_t(new QPixmap(file.absoluteFilePath()))});
    return 0;
}

int CImgMgr::cacheNext(QFileInfo file)
{
    m_cache.push_back({file, pImg_t(new QPixmap(file.absoluteFilePath()))});
    return 0;
}

bool CImgMgr::isWholeDirCached() const
{
    return m_cache.size() >= static_cast<cache_t::size_type>(m_fileMgr->size());
}
