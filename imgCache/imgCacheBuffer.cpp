#include "imgCacheBuffer.h"
#include <QDebug>

CImgCacheBuffer::CImgCacheBuffer()
{

}

CImgCacheBuffer::~CImgCacheBuffer()
{

}

int CImgCacheBuffer::cache(QPixmap &img)
{
    //TODO: 暂时处理非环形版本
    if (m_idxBegin == BUF_SIZE)
        return -1;
    m_buf.at(m_idxBegin).reset(new QPixmap(img));
    qDebug() << m_idxBegin << ": " << m_buf.at(m_idxBegin).get()
             <<"(" << m_buf.at(m_idxBegin)->size();
    ++m_idxBegin;
    return 0;
}
