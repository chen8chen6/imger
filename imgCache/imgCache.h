#ifndef CIMGCACHE_H
#define CIMGCACHE_H

#include <memory> //std::pair, std::shared_ptr
#include <QFileInfo>


class CFileMgr;
class CImgCacheBuffer;

class CImgCache
{
public:
    typedef struct tag_imgFile
    {
        QFileInfo m_info;
        std::shared_ptr<QPixmap> m_pImg;
    } TImgFile;

public:
    CImgCache();
    virtual ~CImgCache();

    int init(const QString &imgPath);
    TImgFile cur(void);
    TImgFile prev(void);
    TImgFile next(void);

private:
    std::shared_ptr<CFileMgr> m_fileMgr = nullptr;  //文件管理
    std::shared_ptr<CImgCacheBuffer> m_buf = nullptr;  //缓存
    QFileInfo m_curFile;
};

#endif // CIMGCACHE_H
