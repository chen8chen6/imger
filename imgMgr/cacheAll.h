#ifndef CCACHEALL_H
#define CCACHEALL_H

#include "imgMgr.h"

class CCacheAll : public CImgMgr
{

public:
    CCacheAll();

    QString name(void) const override { return "imgMgr: Cache all"; }
    int init(const QString& imgPath, std::shared_ptr<CFileMgr> fileMgr) override;
    pImgFile_t cur(void) const override;
    pImgFile_t prev(void) override;
    pImgFile_t next(void) override;

private:
    typedef std::list<pImgFile_t> cache_t;
    cache_t m_cache;        //缓存
    cache_t::iterator m_cur;//当前图片文件
};

#endif // CCACHEALL_H
