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
        m_cur = std::find(m_fileList.begin(), m_fileList.end(), imgFile);
        if (m_fileList.end() == m_cur)
            break;  //文件非图片

        isSucc = true;
    } while(0);

    return isSucc ? 0 : -1;
}

QFileInfo CFileMgr::next()
{
    moveForward();
    return *m_cur;
}

QFileInfoList CFileMgr::next(int num)
{
    //TODO: 处理num > list.size
    QFileInfoList res;
    for (int i = 0; i < num; ++i)
    {
        moveForward();
        res.push_back(*m_cur);
    }
    return res;
}

void CFileMgr::moveForward(int d)
{
    //TODO: 使用distance()去除循环
    //TODO: 对d=1优化计算
    //TODO: realDistance = d%list.size()
    for (int i = 0; i < d; ++i)
    {
        ++m_cur;
        if (m_fileList.end() == m_cur)
            m_cur = m_fileList.begin();
    }
    return;
}
