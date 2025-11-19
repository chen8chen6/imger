#ifndef CIMGCACHEBUFFER_H
#define CIMGCACHEBUFFER_H

#include <array>
#include <memory>   //std::shared_ptr
#include <QPixmap>

class CImgCacheBuffer
{
public:
    static constexpr int BUF_SIZE = 2;
    typedef std::shared_ptr<QPixmap> pImg_t;

public:
    CImgCacheBuffer();
    virtual ~CImgCacheBuffer();

    int cache(QPixmap &img);
    pImg_t first(void) const {return *m_buf.begin();}

private:

    //环形缓冲
    typedef std::array<pImg_t, BUF_SIZE> buf_t;
    buf_t m_buf;
    buf_t::size_type m_idxBegin = 0;
    buf_t::size_type m_idxEnd = BUF_SIZE;
};

#endif // CIMGCACHEBUFFER_H
