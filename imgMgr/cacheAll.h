#ifndef CCACHEALL_H
#define CCACHEALL_H

#include "imgMgr.h"

class CCacheAll : public CImgMgr
{

public:
    CCacheAll();

    QString name(void) const override {return "imgMgr: Cache all";}
    int init(const QString &imgPath, std::shared_ptr<CFileMgr> fileMgr) override;
    TImgFile cur(void) const override;
    TImgFile prev(void) override;
    TImgFile next(void) override;

private:
    typedef std::list<TImgFile> cache_t;
    cache_t m_cache;        //缓存
    cache_t::iterator m_cur;//当前图片文件
};

#endif // CCACHEALL_H
