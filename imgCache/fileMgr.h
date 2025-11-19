#ifndef CFILEMGR_H
#define CFILEMGR_H

#include <QFileInfoList>

//TODO: 目前的设计实际上不能处理用户反复在2张图片切换时的缓冲返回
class CFileMgr
{
public:
    CFileMgr();
    virtual ~CFileMgr();

    int init(const QString &imgPath);
    QFileInfo prev(void);   //TODO: 返回cur的前n个文件?
    QFileInfo next(void);
    int size(void) const {return m_fileList.size();}

private:
    QFileInfoList m_fileList;   //文件列表
    QFileInfoList::iterator m_prev;
    QFileInfoList::iterator m_next;
};

#endif // CFILEMGR_H
