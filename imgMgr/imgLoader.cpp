#include "imgLoader.h"
#include <QDebug>
#include <QPixmap>
#include "imgType.h"

CImgLoader::CImgLoader()
{

}

void CImgLoader::onSigLoadImg(QVariant var)
{
    if (!var.canConvert<TImgFile>())
        return;

    //读取图片并填充进传入的指针
    auto imgFile = var.value<TImgFile>();
    QPixmap img(imgFile.m_info.absoluteFilePath());

    std::unique_lock<std::mutex> locker(*(imgFile.m_mutex));
    *(imgFile.m_pImg) = img;
    locker.unlock();    //主动释放, 不然qDebug也会耗时

    qDebug() << "load over: " << imgFile.m_info.fileName();

    return;
}
