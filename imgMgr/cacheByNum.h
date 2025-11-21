#ifndef CCACHEBYNUM_H
#define CCACHEBYNUM_H

#include "imgMgr.h"

//初始化完成后, 缓冲区始终都是满的, 且m_cur不会位于列表头尾
class CCacheByNum : public CImgMgr
{
public:
    CCacheByNum();
    ~CCacheByNum() override;

    //缓冲池大小
    static constexpr int cacheSize(void) {return FORWARD_CACHE_SIZE + BACKWARD_CACHE_SIZE + 1;}

    QString name(void) const override {return "imgMgr: Cache by num";}
    int init(const QString &imgPath, std::shared_ptr<CFileMgr> fileMgr) override;
    pImgFile_t cur(void) const override { return *m_cur;}
    pImgFile_t prev(void) override;
    pImgFile_t next(void) override;

private:
    static constexpr int FORWARD_CACHE_SIZE = 10;   //向后缓存数量
    static constexpr int BACKWARD_CACHE_SIZE = 9;   //向前缓存数量
    typedef std::list<pImgFile_t> cache_t;    //应该插智能指针, 防止出cache后imgLoader还想改

private:
    int cachePrev(QFileInfo file);
    int cacheNext(QFileInfo file);

private:
    cache_t m_cache;        //缓存
    cache_t::iterator m_cur;//当前图片文件
    std::shared_ptr<CFileMgr> m_fileMgr = nullptr;

};

#endif // CCACHEBYNUM_H
