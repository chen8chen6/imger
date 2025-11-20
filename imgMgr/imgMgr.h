#ifndef CIMGMGR_H
#define CIMGMGR_H

#include <QThread>
#include <QVariant>
#include "imgType.h"

class CFileMgr;
class CImgMgr : public QObject
{
    Q_OBJECT
public:
    CImgMgr();
    virtual ~CImgMgr();

    virtual QString name(void) const = 0;
    virtual int init(const QString &imgPath, std::shared_ptr<CFileMgr> fileMgr) = 0;
    virtual TImgFile cur(void) const = 0;
    virtual TImgFile prev(void) = 0;
    virtual TImgFile next(void) = 0;

signals:
    void sigLoadImg(QVariant var);

protected:
    int initImgLoaderThr(void);
    int uninitImgLoaderThr(void);

protected:
    QThread m_imgLoaderThr;
};

//图像管理类的工厂
class CImgMgrFac
{
public:
    static std::shared_ptr<CImgMgr> create(int fileNum);

private:
    CImgMgrFac() = delete;
    ~CImgMgrFac() = delete;
};

#endif // CIMGMGR_H
