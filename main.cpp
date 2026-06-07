#include <QApplication>
#include <QMessageBox>
#include "imgView.h"
#include "cfgMgr.h"
#include "cfgDlg.h"

char** argv2Thin(int argc, wchar_t* argv[])
{
    char** thinArgv = new char* [argc + 1];    //argv[argc] = NULL
    for (int i = 0; i < argc; ++i)
    {
        static constexpr int BUF_SIZE = 256;
        thinArgv[i] = new char[BUF_SIZE];
        snprintf(thinArgv[i], BUF_SIZE, QString::fromWCharArray(argv[i]).toLocal8Bit().constData());
        thinArgv[i][BUF_SIZE - 1] = '\0';
    }
    thinArgv[argc] = NULL;

    return thinArgv;
}

void delThinArgv(char* argv[])
{
    int i = 0;
    while (NULL != argv[i]) //argv[]的最后一个元素会是NULL
    {
        delete[] argv[i];
        ++i;
    }
    return;
}

int wmain(int argc, wchar_t* argv[])
{
    //使用main(int, char*[])作为主函数时, argv里是gbk编码, 部分无法以gbk编码显示的字符会丢失,
    //因此此处使用宽字符版本的main函数,此时 wchar_t *argv[]里是unicode编码的宽字符
    //这里转成gbk编码格式的char *[]类型用于构造QApplication (注意: gbk编码里无法表示的字符会被转化成'?')
    //此处传递char* thinArgv[]给QApplication, 目的仅为保证从wmain传入的参数不至于完全丢失
    std::shared_ptr<char*> thinArgv{ argv2Thin(argc, argv), delThinArgv };
    QApplication a(argc, thinArgv.get());

    //获取传入路径
    if (argc < 2)
    {
        QMessageBox::critical(nullptr, "错误", "未传入路径");
        return -1;
    }

    const QString imgPath = QString::fromWCharArray(argv[1]);
    //载入配置
    CFG::CCfgMgr* cfgMgr = CFG::CCfgMgr::getSingleton();
    if (0 != cfgMgr->load())
    {
        cfgMgr->reset();
        cfgMgr->save();
        cfgMgr->load();
    }

    //无框, 占满屏幕
    CImgView imgView;
    imgView.setWindowFlag(Qt::FramelessWindowHint, true);
    if (0 != imgView.init(imgPath, cfgMgr))
    {
        QMessageBox::critical(nullptr, "错误", "初始化失败");
        return -1;
    }
    imgView.showFullScreen();

    return a.exec();
}
