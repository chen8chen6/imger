#ifndef CIMGCACHEBUFFER_H
#define CIMGCACHEBUFFER_H

#include <QPixmap>
#include "imgType.h"

class CImgCacheBuffer
{
public:
    static constexpr int BUF_SIZE = 8;

public:
    CImgCacheBuffer();
    virtual ~CImgCacheBuffer();

    int cache(const QFileInfo &info, QPixmap img);
    TImgFile cur(void) const {return *m_buf.begin();}
    TImgFile next(void);

private:
    //TODO: backwardBuf, m_cur, forwardBuf
    typedef std::list<TImgFile> buf_t;
    buf_t m_buf;
};

#endif // CIMGCACHEBUFFER_H
