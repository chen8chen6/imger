#include "cfgHelper.h"

#include <QKeyEvent>
#include <QDebug>

#define USAGE_DESC(usage, desc) \
    { Usage::##usage,QString::fromUtf8(desc) }

#define KEY_DESC(key, desc) \
    { Qt::##key, QStringLiteral(desc) }


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

    bool CCfgHelper::isModKey(int key)
    {
        return (Qt::Key_Control == key
            || Qt::Key_Alt == key
            || Qt::Key_Shift == key);
    }

    QString CCfgHelper::desc(const DspStgy::Stgy stgy)
    {
        static const QString descs[] = { QString::fromUtf8("原始大小"), QString::fromUtf8("适应窗口") };
        const int cnt = sizeof(descs) / sizeof(descs[0]);
        const int idx = stgy;
        return (0 <= idx && idx < cnt) ? descs[idx] : QStringLiteral("Unknown");
    }

    QString CCfgHelper::desc(const DspOrder::Order order)
    {
        static const QString descs[] = { QString::fromUtf8("按文件名"), QString::fromUtf8("按时间") };
        const int cnt = sizeof(descs) / sizeof(descs[0]);
        const int idx = order;
        return (0 <= idx && idx < cnt) ? descs[idx] : QStringLiteral("Unknown");
    }

    QString CCfgHelper::desc(const Usage usage)
    {
        static const std::map<Usage, QString> dict = {
            USAGE_DESC(Help,        "帮助"),
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
        //TODO: 用qDebug脸滚键盘然后生成dict
        const std::map<Qt::Key, QString> dict = {
            KEY_DESC(Key_0, "0"),
            KEY_DESC(Key_Plus, "+"),
            KEY_DESC(Key_Minus, "-"),
            KEY_DESC(Key_Equal, "="),
            KEY_DESC(Key_Left, "←"),
            KEY_DESC(Key_Right, "→"),
            KEY_DESC(Key_Up, "↑"),
            KEY_DESC(Key_Down, "↓"),
            KEY_DESC(Key_Z, "z"),
            KEY_DESC(Key_X, "x"),
        };
        return (dict.count(key) > 0) ? dict.at(key) : QStringLiteral("unknown");
    }

    bool CCfgHelper::isUsageType(Usage usage, Usage usageType)
    {
        return static_cast<int>(usageType) == (static_cast<int>(usage) & static_cast<int>(usageType));
    }
}
