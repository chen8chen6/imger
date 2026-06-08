#ifndef IMGTYPE_H
#define IMGTYPE_H

#include <memory>   //std::shared_ptr
#include <atomic>   //std::atomic_bool
#include <QFileInfo>
#include <QPixmap>
#include <QMovie>
#include <QBuffer>

class QLabel;
class CAbstractImg;


//抽象的图像类, 负责储存已载入的动图/静图数据
typedef std::shared_ptr<CAbstractImg> pImg_t;
class CAbstractImg
{
public:
    static pImg_t create(const QFileInfo& file);    //生成对应的子类对象

    CAbstractImg(void) {}
    virtual ~CAbstractImg(void) {}
    virtual void displayedBy(QLabel* lb) const = 0;
    virtual pImg_t scaled(int w, int h) const = 0;
    virtual pImg_t scaledToWidth(int w) const = 0;
    virtual pImg_t scaledToHeight(int h) const = 0;
    virtual int width(void) const = 0;
    virtual int height(void) const = 0;
    virtual QSize size(void) const = 0;
};

//用来储存静态图像(png等)
class CPicture : public CAbstractImg
{
public:
    CPicture(QPixmap&& pic);
    void displayedBy(QLabel* lb) const override;
    pImg_t scaled(int w, int h) const override;
    pImg_t scaledToWidth(int w) const override;
    pImg_t scaledToHeight(int h) const override;
    int width(void) const override;
    int height(void) const override;
    QSize size(void) const override;
private:
    QPixmap m_pic;
};

//用来储存动态图像(gif等)
class CMovie : public CAbstractImg
{
public:
    CMovie(const QString& filePath);
    CMovie(const CMovie& other);
    void displayedBy(QLabel* lb) const override;
    pImg_t scaled(int w, int h) const override;
    pImg_t scaledToWidth(int w) const override;
    pImg_t scaledToHeight(int h) const override;
    int width(void) const override;
    int height(void) const override;
    QSize size(void) const override;
    CMovie& operator=(const CMovie&) = delete;

private:
    QBuffer m_raw;      //QMovie不支持直接复制, 因此将生成QMovie的源数据保存在内存中
    std::shared_ptr<QMovie> m_movie;
};

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
    pImg_t m_pImg;
    std::atomic_bool m_isReady;     //图片是否载入完成
} TImgFile;
typedef std::shared_ptr<TImgFile> pImgFile_t;
Q_DECLARE_METATYPE(pImgFile_t); //注册为QT的metatype, 用于信号传递

#endif // IMGTYPE_H

