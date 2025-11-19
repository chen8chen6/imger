#include "imgCacheBuffer.h"
#include <QDebug>

CImgCacheBuffer::CImgCacheBuffer()
{

}

CImgCacheBuffer::~CImgCacheBuffer()
{

}

int CImgCacheBuffer::cache(const QFileInfo &info, QPixmap img)
{
    //维护缓冲区大小
    while (BUF_SIZE <= m_buf.size())
    {
        m_buf.pop_front();
    }

    qDebug() << "(" << m_buf.size() << ")"
             << info.absoluteFilePath()
             << img.size();
    m_buf.push_back({info, pImg_t(new QPixmap(img))});
    return 0;
}

TImgFile CImgCacheBuffer::next()
{
    static TImgFile empty{QFileInfo(), nullptr};
    if (m_buf.size() < 2)
        return empty;

    m_buf.pop_front();
    return m_buf.front();
}
