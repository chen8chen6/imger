#include "cfgHelper.h"

#include <QKeyEvent>
#include <QDebug>

#define USAGE_DESC(usage, desc) \
    { Usage::##usage,QString::fromUtf8(desc) }

#define KEY_DESC(key, desc) \
    { Qt::##key, QStringLiteral(desc) },


namespace CFG
{

    keyHash_t CCfgHelper::getKeyHash(int key, int modifiler)
    {
        return (static_cast<keyHash_t>(modifiler) << 32) | (key & 0xFFFFFFFF);
    }

    keyHash_t CCfgHelper::getKeyHash(const QKeyEvent* ev)
    {
        return getKeyHash(ev->key(), getMod(ev));
    }

    bool CCfgHelper::isModKey(int key)
    {
        return (Qt::Key_Control == key
            || Qt::Key_Alt == key
            || Qt::Key_Shift == key);
    }

    QString CCfgHelper::desc(const DspStgy stgy)
    {
        static const QString descs[] = { QString::fromUtf8("原始大小"), QString::fromUtf8("适应窗口") };
        const int cnt = sizeof(descs) / sizeof(descs[0]);
        const int idx = static_cast<int>(stgy);
        return (0 <= idx && idx < cnt) ? descs[idx] : QStringLiteral("Unknown");
    }

    QString CCfgHelper::desc(const DspOrder order)
    {
        static const QString descs[] = { QString::fromUtf8("按文件名"), QString::fromUtf8("按时间") };
        const int cnt = sizeof(descs) / sizeof(descs[0]);
        const int idx = static_cast<int>(order);
        return (0 <= idx && idx < cnt) ? descs[idx] : QStringLiteral("Unknown");
    }

    QString CCfgHelper::desc(const Usage usage)
    {
        static const std::map<Usage, QString> dict = {
            USAGE_DESC(Help,        "帮助"),
            USAGE_DESC(Config,      "设置"),
            USAGE_DESC(Copy,        "复制"),
            USAGE_DESC(Del,         "删除"),
            USAGE_DESC(Next_Img,    "下一张图片"),
            USAGE_DESC(Prev_Img,    "上一张图片"),
            USAGE_DESC(ZoomIn,      "放大"),
            USAGE_DESC(ZoomOut,     "缩小"),
            USAGE_DESC(ZoomReset,   "原始大小"),
            USAGE_DESC(SightUp,     "视线上移"),
            USAGE_DESC(SightDown,   "视线下移"),
            USAGE_DESC(SightLeft,   "视线左移"),
            USAGE_DESC(SightRight,  "视线右移"),
            USAGE_DESC(SightUp_1px, "视线上移(1像素)"),
            USAGE_DESC(SightDown_1px,   "视线下移(1像素)"),
            USAGE_DESC(SightLeft_1px,   "视线左移(1像素)"),
            USAGE_DESC(SightRight_1px,  "视线右移(1像素)"),
        };
        return (dict.count(usage) > 0) ? dict.at(usage) : QStringLiteral("unknown");
    }

    QString CCfgHelper::desc(const keyHash_t keyHash)
    {
        Qt::Key key = static_cast<Qt::Key>(keyHash & 0xFFFFFFFF);
        int mod = (keyHash >> 32);

        QString res;
        if (MOD_CTRL == (MOD_CTRL & mod))
            res.append("Ctrl+");
        if (MOD_ALT == (MOD_ALT & mod))
            res.append("Alt+");
        if (MOD_SHIFT == (MOD_SHIFT & mod))
            res.append("Shift+");
        if (!isModKey(key) && 0 != key)    //TODO: magic_num不是解决之道
            res.append(QStringLiteral("'%1'").arg(desc(key)));

        return res;
    }

    QString CCfgHelper::desc(const Qt::Key key)
    {
        const std::map<Qt::Key, QString> dict = {
            KEY_DESC(Key_Escape,"esc")
            KEY_DESC(Key_F1,"F1")
            KEY_DESC(Key_F2,"F2")
            KEY_DESC(Key_F3,"F3")
            KEY_DESC(Key_F4,"F4")
            KEY_DESC(Key_F5,"F5")
            KEY_DESC(Key_F6,"F6")
            KEY_DESC(Key_F7,"F7")
            KEY_DESC(Key_F8,"F8")
            KEY_DESC(Key_F9,"F9")
            KEY_DESC(Key_F10,"F10")
            KEY_DESC(Key_F11,"F11")
            KEY_DESC(Key_F12,"F12")
            KEY_DESC(Key_Insert,"insert")
            KEY_DESC(Key_Delete,"del")
            KEY_DESC(Key_AsciiTilde,"~")
            KEY_DESC(Key_Exclam,"!")
            KEY_DESC(Key_At,"@")
            KEY_DESC(Key_NumberSign,"#")
            KEY_DESC(Key_Dollar,"$")
            KEY_DESC(Key_Percent,"%")
            KEY_DESC(Key_AsciiCircum,"^")
            KEY_DESC(Key_Ampersand,"&")
            KEY_DESC(Key_Asterisk,"*")
            KEY_DESC(Key_ParenLeft,"(")
            KEY_DESC(Key_ParenRight,")")
            KEY_DESC(Key_Underscore,"_")
            KEY_DESC(Key_Plus,"+")
            KEY_DESC(Key_Backspace,"backspace")
            KEY_DESC(Key_QuoteLeft,"`")
            KEY_DESC(Key_1,"1")
            KEY_DESC(Key_2,"2")
            KEY_DESC(Key_3,"3")
            KEY_DESC(Key_4,"4")
            KEY_DESC(Key_5,"5")
            KEY_DESC(Key_6,"6")
            KEY_DESC(Key_7,"7")
            KEY_DESC(Key_8,"8")
            KEY_DESC(Key_9,"9")
            KEY_DESC(Key_0,"0")
            KEY_DESC(Key_Minus,"-")
            KEY_DESC(Key_Equal,"=")
            KEY_DESC(Key_Plus,"+")
            KEY_DESC(Key_Underscore,"_")
            KEY_DESC(Key_Tab,"tab")
            KEY_DESC(Key_CapsLock,"caps lock")
            //KEY_DESC(Key_Meta,"") //windows徽标键 
            KEY_DESC(Key_BraceLeft,"{")
            KEY_DESC(Key_BraceRight,"}")
            KEY_DESC(Key_Bar,"|")
            KEY_DESC(Key_QuoteDbl,"\"")
            KEY_DESC(Key_Colon,":")
            KEY_DESC(Key_Less,"<")
            KEY_DESC(Key_Greater,">")
            KEY_DESC(Key_Question,"?")
            KEY_DESC(Key_BracketLeft,"[")
            KEY_DESC(Key_BracketRight,"]")
            KEY_DESC(Key_Backslash,"\\")
            KEY_DESC(Key_Apostrophe,"'")
            KEY_DESC(Key_Semicolon,";")
            KEY_DESC(Key_Comma,",")
            KEY_DESC(Key_Period,".")
            KEY_DESC(Key_Slash,"/")
            KEY_DESC(Key_Q,"Q")
            KEY_DESC(Key_A,"A")
            KEY_DESC(Key_Z,"Z")
            KEY_DESC(Key_X,"X")
            KEY_DESC(Key_S,"S")
            KEY_DESC(Key_W,"W")
            KEY_DESC(Key_E,"E")
            KEY_DESC(Key_D,"D")
            KEY_DESC(Key_C,"C")
            KEY_DESC(Key_V,"V")
            KEY_DESC(Key_F,"F")
            KEY_DESC(Key_R,"R")
            KEY_DESC(Key_T,"T")
            KEY_DESC(Key_G,"G")
            KEY_DESC(Key_B,"B")
            KEY_DESC(Key_N,"N")
            KEY_DESC(Key_H,"H")
            KEY_DESC(Key_Y,"Y")
            KEY_DESC(Key_U,"U")
            KEY_DESC(Key_J,"J")
            KEY_DESC(Key_M,"M")
            KEY_DESC(Key_K,"K")
            KEY_DESC(Key_I,"I")
            KEY_DESC(Key_O,"O")
            KEY_DESC(Key_P,"P")
            KEY_DESC(Key_L,"L")
            KEY_DESC(Key_Space," ")
            KEY_DESC(Key_Up,"↑")
            KEY_DESC(Key_Left,"←")
            KEY_DESC(Key_Down,"↓")
            KEY_DESC(Key_Right,"→")
        };
        return (dict.count(key) > 0) ? dict.at(key) : QStringLiteral("unknown");
    }

    int CCfgHelper::getMod(const QKeyEvent* ev)
    {
        int mod = NO_MODIFILER;
        auto modFlags = ev->modifiers();
        if (modFlags.testFlag(Qt::ControlModifier))
            mod |= MOD_CTRL;
        if (modFlags.testFlag(Qt::AltModifier))
            mod |= MOD_ALT;
        if (modFlags.testFlag(Qt::ShiftModifier))
            mod |= MOD_SHIFT;
        return mod;
    }

    bool CCfgHelper::isUsageType(Usage usage, Usage usageType)
    {
        return static_cast<int>(usageType) == (static_cast<int>(usage) & static_cast<int>(usageType));
    }
}
