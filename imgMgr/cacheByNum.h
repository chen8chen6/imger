#ifndef CCACHEBYNUM_H
#define CCACHEBYNUM_H

#include "imgMgr.h"

/**
* 缓冲区结构如下图所示:
* 前<-----     ----->后
* |prev_most|----|prev_avg|----|prev_least|----|cur|----|next_least|----|next_avg|----|next_most|
* 
* prev_most: 向前缓存至多prev_most个元素, 高于该水平则删减到prev_avg个元素.
* prev_avg: 向前缓存的元素数量高于prev_most或低于prev_least时, 调整至prev_avg水平.
* prev_least: 向前缓存至少prev_least个元素, 低于该水平则补充到prev_avg个元素.
* cur: 当前指针
* next_most: 向后缓存至少next_least个元素, 低于该水平则补充到next_avg个元素.
* next_avg: 向后缓存的元素数量高于next_most或低于next_least时, 调整至next_avg水平.
* next_most: 向后缓存至多next_most个元素, 高于该水平则删减到next_avg个元素.
**/
class CCacheByNum : public CImgMgr
{
public:
    CCacheByNum();
    ~CCacheByNum() override;

    //缓冲池大小
    //小于该数目的话, 会使用cacheAll策略, 因此这里不会出现向前向后缓冲到同一个文件的情况
    static constexpr int cacheSize(void) {return NEXT_CACHE_MOST + PREV_CACHE_MOST + 1;}

    QString name(void) const override {return "imgMgr: Cache by num";}
    int init(const QString &imgPath, std::shared_ptr<CFileMgr> fileMgr) override;
    pImgFile_t cur(void) const override { return *m_cur;}
    pImgFile_t prev(void) override;
    pImgFile_t next(void) override;

private:
    //向后缓存数量
    static constexpr int NEXT_CACHE_LEAST = 10;
    static constexpr int NEXT_CACHE_MOST = 20;
    static constexpr int NEXT_CACHE_AVG = (NEXT_CACHE_MOST + NEXT_CACHE_LEAST) / 2;

    //向前缓存数量
    static constexpr int PREV_CACHE_LEAST = 5;
    static constexpr int PREV_CACHE_MOST = 15;
    static constexpr int PREV_CACHE_AVG = (PREV_CACHE_MOST + PREV_CACHE_LEAST) / 2;

    typedef std::list<pImgFile_t> cache_t;

private:
    int cachePrev(QFileInfo file);
    int cacheNext(QFileInfo file);

private:
    cache_t m_cache;        //缓存
    int m_nextCached = 0;   //向后已缓存数量
    int m_prevCached = 0;   //向前已缓存数量

    cache_t::iterator m_cur;//当前图片文件
    std::shared_ptr<CFileMgr> m_fileMgr = nullptr;
};

#endif // CCACHEBYNUM_H
