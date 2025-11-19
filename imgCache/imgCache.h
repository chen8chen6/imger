#ifndef CIMGCACHE_H
#define CIMGCACHE_H

#include <memory> //std::pair
#include <QFileInfo>
//#include <QPixmap>

class CFileMgr;
class CImgCacheBuffer;

class CImgCache
{
public:
    typedef std::pair<QFileInfo, QPixmap *> imgFile_t;

public:
    CImgCache();
    virtual ~CImgCache();

    int init(const QString &imgPath);
    imgFile_t cur(void);
    imgFile_t prev(void);
    imgFile_t next(void);

private:
    CFileMgr *m_fileMgr = nullptr;  //文件管理
    CImgCacheBuffer *m_buf = nullptr;  //缓存
    QFileInfo m_curFile;
};

#endif // CIMGCACHE_H
