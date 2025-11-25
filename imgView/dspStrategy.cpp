#include "dspStrategy.h"
#include <QPixmap>

CDspStrategy::~CDspStrategy()
{

}

CZoomToSize::CZoomToSize(int width, int height)
    : m_width(width), m_height(height),
      m_whRatio(height <= 0.0 ? 1.0 : width * 1.0/height)
{

}

int CZoomToSize::process(QPixmap *img)
{
    if (0 == img->width() || 0 == img->height())
        return -1;

    //按比例缩放
    double img_whRatio = img->width() * 1.0 / img->height();
    bool isTooWide = img_whRatio > m_whRatio;
    *img = isTooWide
            ? img->scaledToWidth(m_width, Qt::SmoothTransformation)
            : img->scaledToHeight(m_height, Qt::SmoothTransformation);

    return 0;
}
