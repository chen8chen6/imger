#ifndef CFILEMGR_H
#define CFILEMGR_H

#include <QFileInfoList>

class CFileMgr
{
public:
    CFileMgr();
    virtual ~CFileMgr();

    int init(const QString &filePath, const QDir &dir);
    QFileInfoList::size_type size(void) const {return m_fileList.size();}
    bool exist(const QFileInfo &file) const;

    //获取指定文件前/后的n个文件
    QFileInfo fileBefore(const QFileInfo &file);
    QFileInfoList nFilesBefore(int n, const QFileInfo &file);
    QFileInfo fileAfter(const QFileInfo &file);
    QFileInfoList nFilesAfter(int n, const QFileInfo &file);

    QFileInfoList allFiles(void) {return m_fileList;}

private:
    //按前缀+序号的方式排序, 处理按文件名排序时出现10.jpg插入1.jpg和2.jpg之间的问题
    static bool sortByPrefixAndIdx(const QFileInfo &l, const QFileInfo &r);
    static void divide(const QString &origin, QString &prefix, QString &idx, QString &remain);

    //查找指定文件在列表中的位置
    //QFileInfoList::iterator fileIter(const QFileInfo &file);
    QFileInfoList::const_iterator fileConstIter(const QFileInfo &file) const;

    //移动迭代器, 如果到达末尾, 则移动到最前端
    void moveForward(QFileInfoList::const_iterator &iter) const;
    void moveBackward(QFileInfoList::const_iterator &iter) const;

private:
    QFileInfoList m_fileList;   //文件列表
    //TODO: 使用window加速搜索

};

#endif // CFILEMGR_H
