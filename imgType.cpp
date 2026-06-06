#include "imgType.h"

tag_imgFile::tag_imgFile(QFileInfo info, std::shared_ptr<QPixmap> pImg, bool isReady)
{
    m_info = info;
    m_pImg = pImg;
    std::atomic_init(&m_isReady, isReady);
}

tag_imgFile::tag_imgFile(const tag_imgFile& other)
{
    *this = other;
}

tag_imgFile::tag_imgFile(tag_imgFile&& other)
{
    *this = std::move(other);
}

tag_imgFile& tag_imgFile::operator=(const tag_imgFile& other)
{
    m_info = other.m_info;
    m_pImg = other.m_pImg;
    bool isReady = other.m_isReady;
    m_isReady = isReady;
    return *this;
}

tag_imgFile& tag_imgFile::operator=(tag_imgFile&& other)
{
    m_info.swap(other.m_info);
    m_pImg.swap(other.m_pImg);
    bool isReady = other.m_isReady;
    m_isReady = isReady;
    return *this;
}
