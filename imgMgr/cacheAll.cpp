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
        //这个策略里, 缓存完成后只读不写
        pImgFile_t pImgFile(new TImgFile(file, pImg_t(new QPixmap(file.absoluteFilePath()))));
        pImgFile->m_isReady = true;
        m_cache.push_back(std::move(pImgFile));
    }

    //定位迭代器
    QFileInfo curImg(imgPath);
    m_cur = std::find_if(m_cache.begin(), m_cache.end(),
                         [&](const pImgFile_t &pImg){ return pImg->m_info == curImg; });
    if (m_cache.end() == m_cur)
        return -1;  //文件未找到

    return 0;
}

pImgFile_t CCacheAll::cur() const
{
    return *m_cur;
}

pImgFile_t CCacheAll::prev()
{
    if (m_cache.begin() == m_cur)
        m_cur = m_cache.end();
    --m_cur;
    return *m_cur;
}

pImgFile_t CCacheAll::next()
{
    if (m_cache.end() == ++m_cur)
        m_cur = m_cache.begin();
    return *m_cur;
}
