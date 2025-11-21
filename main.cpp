#include <QApplication>
#include <QMessageBox>
#include "imgView.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //获取传入路径
    if (argc < 2)
    {
        QMessageBox::critical(nullptr, "错误", "未传入路径");
        return -1;
    }
    const QString imgPath = QString::fromLocal8Bit(argv[1]);

    //无框, 占满屏幕
    CImgView imgView;
    imgView.setWindowFlag(Qt::FramelessWindowHint, true);
    if (0 != imgView.init(imgPath))
    {
        QMessageBox::critical(nullptr, "错误", "初始化失败");
        return -1;
    }
    imgView.showFullScreen();

    return a.exec();
}
