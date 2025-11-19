#ifndef CFILEMGR_H
#define CFILEMGR_H

#include <QFileInfoList>

//TODO: 目前的设计实际上不能处理用户反复在2张图片切换时的缓冲返回
//TODO: fileMgr和cacheBuf的前/后/当前是保持一致的, fileMgr是不是该由cacheBuf持有
class CFileMgr
{
public:
    CFileMgr();
    virtual ~CFileMgr();

    int init(const QString &imgPath);

    //返回前/后n个文件的信息, m_cur也会移动相应的距离
    QFileInfo prev(void);
    QFileInfo next(void);
    QFileInfoList next(int num);

    //TODO: 提供仅移动m_cur指针的方法, 但名字要合理
    int size(void) const {return m_fileList.size();}

private:
    void moveForward(int d = 1);    //TODO: rename, 而且要显示指出m_cur会变化

private:
    QFileInfoList m_fileList;   //文件列表
    QFileInfoList::iterator m_cur;  //类初始化后, 始终指向有效元素
};

#endif // CFILEMGR_H
