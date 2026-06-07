#ifndef IMGTYPE_H
#define IMGTYPE_H

#include <memory>   //std::shared_ptr
#include <atomic>   //std::atomic_bool
#include <QFileInfo>


class QPixmap;
typedef std::shared_ptr<QPixmap> pImg_t;

//包含文件信息和已载入的图片数据
typedef struct tag_imgFile
{
public:
    tag_imgFile(const QFileInfo& info = QFileInfo(),
        const pImg_t& pImg = nullptr,
        bool isReady = false);
    tag_imgFile(const tag_imgFile& other);
    tag_imgFile(tag_imgFile&& other) noexcept;

    tag_imgFile& operator= (const tag_imgFile& other);
    tag_imgFile& operator= (tag_imgFile&& other) noexcept;

public:
    QFileInfo m_info;
    std::shared_ptr<QPixmap> m_pImg;
    std::atomic_bool m_isReady;     //图片是否载入完成
} TImgFile;
typedef std::shared_ptr<TImgFile> pImgFile_t;
Q_DECLARE_METATYPE(pImgFile_t); //注册为QT的metatype, 用于信号传递

#endif // IMGTYPE_H

