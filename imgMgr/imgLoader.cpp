#include "imgLoader.h"
#include <chrono>
#include <QDebug>
#include <QPixmap>
#include "imgType.h"

using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

CImgLoader::CImgLoader()
{

}

void CImgLoader::onSigLoadImg(QVariant var_pImgFile)
{
    if (!var_pImgFile.canConvert<pImgFile_t>())
        return;

    //读取图片并填充进传入的指针
    auto start = steady_clock::now();   //计时开始
    auto pImgFile = var_pImgFile.value<pImgFile_t>();
    QPixmap img(pImgFile->m_info.absoluteFilePath());
    *(pImgFile->m_pImg) = img;
    pImgFile->m_isReady = true;
    auto cost_ms = duration_cast<milliseconds>(steady_clock::now() - start);

    qDebug() << "loaded(" << cost_ms.count() << "ms): "
        << pImgFile->m_info.fileName()
        << pImgFile->m_pImg->size();
    return;
}
