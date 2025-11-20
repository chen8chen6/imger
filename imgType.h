#ifndef IMGTYPE_H
#define IMGTYPE_H

#include <memory>   //std::shared_ptr
#include <QFileInfo>
#include <mutex>

class QPixmap;

typedef std::shared_ptr<QPixmap> pImg_t;
typedef struct tag_imgFile
{
    QFileInfo m_info;
    std::shared_ptr<QPixmap> m_pImg;
    std::shared_ptr<std::mutex> m_mutex;    //TODO: 这个指针本身并不线程安全, 而且结构体初始化语句写起来特别长
} TImgFile;
Q_DECLARE_METATYPE(TImgFile);   //注册为QT的metatype, 用于信号传递

#endif // IMGTYPE_H
