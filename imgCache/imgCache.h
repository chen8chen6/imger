#ifndef CIMGCACHE_H
#define CIMGCACHE_H

#include "imgType.h"

class CFileMgr;
class CImgCacheBuffer;

class CImgCache
{
public:
    CImgCache();
    virtual ~CImgCache();

    int init(const QString &imgPath);
    TImgFile cur(void) const;
    TImgFile prev(void);
    TImgFile next(void);

private:
    std::shared_ptr<CFileMgr> m_fileMgr = nullptr;  //文件管理
    std::shared_ptr<CImgCacheBuffer> m_buf = nullptr;  //缓存
};

#endif // CIMGCACHE_H
