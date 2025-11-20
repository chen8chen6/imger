#include "cacheAll.h"
#include <QPixmap>
#include <QDebug>
#include "fileMgr.h"

CCacheAll::CCacheAll()
{

}

int CCacheAll::init(const QString &imgPath, std::shared_ptr<CFileMgr> fileMgr)
{
    qDebug() << name();

    //缓存
    QFileInfoList allFiles = fileMgr->allFiles();
    for (const auto &file : allFiles)
    {
        m_cache.push_back({file, pImg_t(new QPixmap(file.absoluteFilePath()))});
    }

    //定位迭代器
    QFileInfo curImg(imgPath);
    m_cur = std::find_if(m_cache.begin(), m_cache.end(),
                         [&](const TImgFile &file){ return file.m_info == curImg; });
    if (m_cache.end() == m_cur)
        return -1;  //文件未找到

    return 0;
}

TImgFile CCacheAll::cur() const
{
    return *m_cur;
}

TImgFile CCacheAll::prev()
{
    if (m_cache.begin() == m_cur)
        m_cur = m_cache.end();
    --m_cur;
    return *m_cur;
}

TImgFile CCacheAll::next()
{
    if (m_cache.end() == ++m_cur)
        m_cur = m_cache.begin();
    return *m_cur;
}
