#include "fileMgr.h"
#include <algorithm>    //std::sort
#include <QDebug>
#include <QDir>
#include <QRegularExpression>

CFileMgr::CFileMgr()
{

}

CFileMgr::~CFileMgr()
{

}

int CFileMgr::init(const QString& filePath, const QDir& dir)
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

        //文件名中的阿拉伯数字串需要按数值大小排序而不是按字符排序
        if (QDir::SortFlag::Name == (dir.sorting() & 0x03))
        {
            std::sort(m_fileList.begin(), m_fileList.end(), sortByPrefixAndIdx);
        }

        isSucc = true;
    } while (0);

    return isSucc ? 0 : -1;
}

bool CFileMgr::exist(const QFileInfo& file) const
{
    return fileConstIter(file) != m_fileList.cend();
}

QFileInfo CFileMgr::fileBefore(const QFileInfo& file)
{
    auto iter = fileConstIter(file);
    moveBackward(iter);
    return *iter;
}

QFileInfoList CFileMgr::nFilesBefore(int n, const QFileInfo& file)
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

QFileInfo CFileMgr::fileAfter(const QFileInfo& file)
{
    auto iter = fileConstIter(file);
    if (m_fileList.cend() == iter)
        return {};

    moveForward(iter);
    return *iter;
}

QFileInfoList CFileMgr::nFilesAfter(int n, const QFileInfo& file)
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

bool CFileMgr::sortByPrefixAndIdx(const QFileInfo& l, const QFileInfo& r)
{
    //TODO: 输入的是一定程度排序后的数组, 相似名称的元素会聚到一起, 理论上应该可以加速
    QString lName = l.fileName(), lPrefix, lIdx, lRemain;
    QString rName = r.fileName(), rPrefix, rIdx, rRemain;

    //将文件名反复拆分成[前缀][序号][其余部分]
    do
    {
        split(lName, lPrefix, lIdx, lRemain);
        split(rName, rPrefix, rIdx, rRemain);

        if (lPrefix != rPrefix || lIdx != rIdx)
            break;

        //前缀和序号一致, 则继续拆分字符串
        lName = lRemain;
        rName = rRemain;
    } while (!lRemain.isEmpty() && !rRemain.isEmpty());

    if (lPrefix == rPrefix && lIdx == rIdx)
        return lRemain.isEmpty() && !rRemain.isEmpty(); //前缀和序号完全一致, 则短字符串优先
    else if (lPrefix != rPrefix)
        return lPrefix < rPrefix;   //对比前缀
    else
        return (lIdx.toInt() == rIdx.toInt())    //对比序号
        ? lIdx < rIdx   //数值一致则按字符串对比
        : lIdx.toInt() < rIdx.toInt();
}

void CFileMgr::split(const QString& origin, QString& prefix, QString& idx, QString& remain)
{
    //(n个non-digit)(n个digit)(其余部分)
    static const QRegularExpression mask{ R"(^(?<prefix>\D*)(?<index>\d*)(?<remain>.*)$)" };

    auto match = mask.match(origin);
    if (match.hasMatch())
    {
        prefix = match.captured("prefix");
        idx = match.captured("index");
        remain = match.captured("remain");
    }
    else
    {
        prefix = origin;
        idx.clear();
        remain.clear();
        qDebug() << "warn: parse [" << origin << "] failed";    //理论上不可能发生
    }

    return;
}

/*QFileInfoList::iterator CFileMgr::fileIter(const QFileInfo &file)
{
    return std::find(m_fileList.begin(), m_fileList.end(), file);
}*/

QFileInfoList::const_iterator CFileMgr::fileConstIter(const QFileInfo& file) const
{
    return std::find(m_fileList.cbegin(), m_fileList.cend(), file);
}

void CFileMgr::moveForward(QFileInfoList::const_iterator& iter) const
{
    if (m_fileList.cend() == ++iter)
        iter = m_fileList.begin();
}

void CFileMgr::moveBackward(QFileInfoList::const_iterator& iter) const
{
    if (m_fileList.begin() == iter)
        iter = m_fileList.cend();
    --iter;
}

