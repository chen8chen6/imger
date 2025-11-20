#ifndef CIMGMGR_H
#define CIMGMGR_H

//#include <QObject>
#include "imgType.h"

class CFileMgr;
class CImgMgr
{
public:
    CImgMgr();

    int init(const QString &imgPath);
    TImgFile cur(void) const { return *m_cur;}
    TImgFile prev(void);
    TImgFile next(void);

private:
    static constexpr int FORWARD_CACHE_SIZE = 10;   //向后缓存数量
    static constexpr int BACKWARD_CACHE_SIZE = 9;   //向前缓存数量
    //static constexpr int CACHE_SIZE = FORWARD_CACHE_NUM + BACKWARD_CACHE_NUM + 1;
    typedef std::list<TImgFile> cache_t;

private:
    int cachePrev(QFileInfo file);
    int cacheNext(QFileInfo file);
    bool isWholeDirCached(void) const;

private:
    cache_t m_cache;        //缓存
    cache_t::iterator m_cur;//当前图片文件
    std::shared_ptr<CFileMgr> m_fileMgr = nullptr;
};

#endif // CIMGMGR_H
