#include "fileMgr.h"
#include <QDebug>
#include <QDir>

CFileMgr::CFileMgr()
{

}

CFileMgr::~CFileMgr()
{

}

int CFileMgr::init(const QString &filePath, const QDir &dir)
{
    bool isSucc = false;
    do
    {
        qDebug() << filePath;

        //读取文件列表
        m_fileList = dir.entryInfoList();
        if (0 == m_fileList.size())
            break;

        //文件不存在或不是支持的图片格式
        QFileInfo imgFile(filePath);
        if (!exist(imgFile))
            break;

        isSucc = true;
    } while(0);

    return isSucc ? 0 : -1;
}

bool CFileMgr::exist(const QFileInfo &file) const
{
    return fileConstIter(file) != m_fileList.cend();
}

QFileInfo CFileMgr::fileBefore(const QFileInfo &file)
{
    auto iter = fileConstIter(file);
    moveBackward(iter);
    return *iter;
}

QFileInfoList CFileMgr::nFilesBefore(int n, const QFileInfo &file)
{
    auto iter = fileConstIter(file);
    if (m_fileList.cend() == iter)
        return {};

    QFileInfoList ret;
    for (int i = 0; i < n; ++i)
    {
        moveBackward(iter);
        ret.push_back(*iter);
    }

    return ret;
}

QFileInfo CFileMgr::fileAfter(const QFileInfo &file)
{
    auto iter = fileConstIter(file);
    if (m_fileList.cend() == iter)
        return {};

    moveForward(iter);
    return *iter;
}

QFileInfoList CFileMgr::nFilesAfter(int n, const QFileInfo &file)
{
    auto iter = fileConstIter(file);
    if (m_fileList.cend() == iter)
        return {};

    QFileInfoList ret;
    for (int i = 0; i < n; ++i)
    {
        moveForward(iter);
        ret.push_back(*iter);
    }

    return ret;
}

/*QFileInfoList::iterator CFileMgr::fileIter(const QFileInfo &file)
{
    return std::find(m_fileList.begin(), m_fileList.end(), file);
}*/

QFileInfoList::const_iterator CFileMgr::fileConstIter(const QFileInfo &file) const
{
    return std::find(m_fileList.cbegin(), m_fileList.cend(), file);
}

void CFileMgr::moveForward(QFileInfoList::const_iterator &iter) const
{
    if (m_fileList.cend() == ++iter)
        iter = m_fileList.begin();
}

void CFileMgr::moveBackward(QFileInfoList::const_iterator &iter) const
{
    if (m_fileList.begin() == iter)
        iter = m_fileList.cend();
    --iter;
}

