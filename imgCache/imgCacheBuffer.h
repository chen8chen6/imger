#ifndef CIMGCACHEBUFFER_H
#define CIMGCACHEBUFFER_H

#include <array>
#include <QPixmap>

class CImgCacheBuffer
{
public:
    static constexpr int BUF_SIZE = 2;

public:
    CImgCacheBuffer();
    virtual ~CImgCacheBuffer();

    int cache(QPixmap &img);
    QPixmap *first(void) const {return *m_buf.begin();}
private:

    //环形缓冲
    typedef std::array<QPixmap *, BUF_SIZE> buf_t;
    buf_t m_buf;
    buf_t::size_type m_idxBegin = 0;
    buf_t::size_type m_idxEnd = BUF_SIZE;
};

#endif // CIMGCACHEBUFFER_H
