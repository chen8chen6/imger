#ifndef IMGTYPE_H
#define IMGTYPE_H

#include <memory>   //std::shared_ptr
#include <QFileInfo>

class QPixmap;

typedef std::shared_ptr<QPixmap> pImg_t;
typedef struct tag_imgFile
{
    QFileInfo m_info;
    std::shared_ptr<QPixmap> m_pImg;
} TImgFile;

#endif // IMGTYPE_H
