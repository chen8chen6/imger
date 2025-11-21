#ifndef CIMGLOADER_H
#define CIMGLOADER_H

#include <QObject>
#include <QVariant>

//TODO: 默认应显示 "loading" 图片
//TODO: 如果imgView当前显示loading, 则load完成后应刷新显示图片
class CImgLoader : public QObject
{
    Q_OBJECT
public:
    CImgLoader();

public slots:
    void onSigLoadImg(QVariant var_pImgFile);

signals:
    void sigLoaded(QVariant var_file);
};

#endif // CIMGLOADER_H
