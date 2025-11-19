#include "fileMgr.h"
#include <QDebug>
#include <QDir>

CFileMgr::CFileMgr()
{

}

CFileMgr::~CFileMgr()
{
    m_fileList.clear();
}

int CFileMgr::init(const QString &imgPath)
{
    bool isSucc = false;
    do
    {
        qDebug() << imgPath;
        QFileInfo imgFile(imgPath);
        if (!imgFile.exists())
            break;  //文件不存在

        //读取文件列表
        QDir dir(imgFile.absolutePath());
        static const QList<QString> imgSuffix = {"*.jpg", "*.jpeg", "*.png", "*.bmp", "*.webp"};
        m_fileList = dir.entryInfoList(imgSuffix, QDir::Files, QDir::Name);
        if (0 == m_fileList.size())
            break;

        //定位迭代器
        m_prev = std::find(m_fileList.begin(), m_fileList.end(), imgFile);
        if (m_fileList.end() == m_prev)
            break;  //文件非图片
        m_next = m_prev;

        isSucc = true;
    } while(0);

    return isSucc ? 0 : -1;
}

QFileInfo CFileMgr::next()
{
    //TODO:注意在fileList.size < buf_size的情况下, next和prev可能会重叠
    if (m_fileList.end() == m_next)
        m_next = m_fileList.begin();
    return *(m_next++);
}
