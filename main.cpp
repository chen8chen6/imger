#include <QApplication>
#include "imgView.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    const QString imgPath = "e:/1.png";

    //无框, 占满屏幕
    CImgView imgView;
    imgView.setWindowFlag(Qt::FramelessWindowHint, true);
    imgView.resize(1920, 1080); //TODO:检测当前屏幕大小
    imgView.init(imgPath);
    imgView.show();

    return a.exec();
}
