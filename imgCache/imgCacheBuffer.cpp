#include "imgCacheBuffer.h"
#include <QDebug>

CImgCacheBuffer::CImgCacheBuffer()
{

}

CImgCacheBuffer::~CImgCacheBuffer()
{
    for (auto pPic: m_buf)
    {
        if (nullptr != pPic)
            delete pPic;
    }
    return;
}

int CImgCacheBuffer::cache(QPixmap &img)
{
    //TODO: 暂时处理非环形版本
    if (m_idxBegin == BUF_SIZE)
        return -1;
    m_buf.at(m_idxBegin) = new QPixmap(img);
    qDebug() << m_idxBegin << ": " << m_buf.at(m_idxBegin)
             <<"(" << m_buf.at(m_idxBegin)->size();
    ++m_idxBegin;
    return 0;
}
