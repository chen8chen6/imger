#include "imgType.h"
#include <QLabel>
#include <QDebug>

tag_imgFile::tag_imgFile(const QFileInfo& info, const pImg_t& pImg, bool isReady)
    : m_info(info), m_pImg(pImg)
{
    std::atomic_init(&m_isReady, isReady);
}


tag_imgFile::tag_imgFile(const tag_imgFile& other)
{
    *this = other;
}

tag_imgFile::tag_imgFile(tag_imgFile&& other) noexcept
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

tag_imgFile& tag_imgFile::operator=(tag_imgFile&& other) noexcept
{
    m_info.swap(other.m_info);
    m_pImg.swap(other.m_pImg);
    bool isReady = other.m_isReady;
    m_isReady = isReady;
    return *this;
}

CPicture::CPicture(QPixmap&& pic)
    : CAbstractImg(), m_pic(pic)
{
}

pImg_t CAbstractImg::create(const QFileInfo& file)
{
    static const auto MOVIE_SUFFIX_LIST = QMovie::supportedFormats();
    QString suffix = file.suffix();
    for (const auto& movSuf : MOVIE_SUFFIX_LIST)
    {
        if (suffix == movSuf)
            return pImg_t(new CMovie(file.absoluteFilePath()));
    }
    return pImg_t(new CPicture(QPixmap(file.absoluteFilePath())));
}

void CPicture::displayedBy(QLabel* lb) const
{
    lb->setPixmap(m_pic);
    if (nullptr != lb->movie())
    {
        lb->movie()->stop();
        lb->setMovie(nullptr);
    }
}

pImg_t CPicture::scaled(int w, int h)const
{
    return pImg_t(new CPicture(m_pic.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
}

pImg_t CPicture::scaledToWidth(int w) const
{
    return pImg_t(new CPicture(m_pic.scaledToWidth(w, Qt::SmoothTransformation)));
}

pImg_t CPicture::scaledToHeight(int h) const
{
    return pImg_t(new CPicture(m_pic.scaledToHeight(h, Qt::SmoothTransformation)));
}

int CPicture::width(void) const
{
    return m_pic.width();
}

int CPicture::height(void) const
{
    return m_pic.height();
}

QSize CPicture::size(void) const
{
    return m_pic.size();
}

CMovie::CMovie(const QString& filePath)
{
    //读取源数据
    QFile movFile(filePath);
    movFile.open(QIODevice::ReadOnly);  //TODO: 处理失败和文件明显异常(体积过大)的情况
    m_raw.setData(movFile.readAll());
    movFile.close();

    m_movie.reset(new QMovie(&m_raw));
    m_movie->setCacheMode(QMovie::CacheAll);
}

CMovie::CMovie(const CMovie& other)
{
    m_raw.setData(other.m_raw.data());
    m_movie.reset(new QMovie(&m_raw));
    m_movie->setCacheMode(QMovie::CacheAll);
}

void CMovie::displayedBy(QLabel* lb) const
{
    lb->setPixmap(QPixmap());
    if (nullptr != lb->movie())
        lb->movie()->stop();

    lb->setMovie(m_movie.get());
    m_movie->start();
}

pImg_t CMovie::scaled(int w, int h) const
{
    CMovie* scaled = new CMovie(*this);
    scaled->m_movie->setScaledSize({ w, h });
    return pImg_t(scaled); //TODO: 使用QBuffer避免再次读硬盘
}

pImg_t CMovie::scaledToWidth(int w) const
{
    int h = height() * w / width(); //TODO: 为什么QMovie只有scaledSize而不是正常的size()? 是不是缩放只针对当前帧有效?
    return scaled(w, h);
}

pImg_t CMovie::scaledToHeight(int h) const
{
    int w = width() * h / height();
    return scaled(w, h);
}

int CMovie::width(void) const
{
    return size().width();
}

int CMovie::height(void) const
{
    return size().height();
}

QSize CMovie::size(void) const
{
    /**
    * scaledSize只在setScaledSize后才会返回有效值, 否则返回{-1, -1}
    * frameRect只在有帧更新后才会返回有效值, 否则返回{0,0,0,0},
    * TODO: 此处默认所有帧size一致, 但理论上每一帧的size可以不同,
    **/
    static const QRect INVALID_FRAME_RECT{ 0,0,0,0 };
    static const QSize INVALID_SIZE{ -1,-1 };
    qDebug() << m_movie->currentFrameNumber()
        << "scaled: " << m_movie->scaledSize()
        << "frm: " << m_movie->frameRect();

    if (INVALID_SIZE != m_movie->scaledSize())
        return m_movie->scaledSize();

    QRect fmRect = m_movie->frameRect();
    if (INVALID_FRAME_RECT != fmRect)
        return { fmRect.width(), fmRect.height() };

    //尝试更新一帧, 更新frameRect
    m_movie->jumpToNextFrame();
    fmRect = m_movie->frameRect();
    qDebug() << m_movie->currentFrameNumber()
        << "scaled: " << m_movie->scaledSize()
        << "frm: " << m_movie->frameRect();
    return { fmRect.width(), fmRect.height() };
}
