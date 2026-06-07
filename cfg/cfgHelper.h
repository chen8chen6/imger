#ifndef CFG_HELPER_H
#define CFG_HELPER_H

#include <QString>
#include "cfgDef.h" //keyHash_t, Usage

#define NO_MODIFILER    0x0
#define MOD_CTRL        0x1
#define MOD_ALT         0x2
#define MOD_SHIFT       0x4

class QKeyEvent;
namespace CFG
{
    class CCfgHelper
    {
    public:
        //将按键及其功能键合并计算为一个64位长整形
        static keyHash_t getKeyHash(int key, int modifiler = NO_MODIFILER);
        static keyHash_t getKeyHash(const QKeyEvent* ev);
        static bool isModKey(int key);

        //判断用途类型
        static bool isWinUsage(Usage usage) { return isUsageType(usage, Usage::Win); }
        static bool isFileUsage(Usage usage) { return isUsageType(usage, Usage::File); }
        static bool isImgUsage(Usage usage) { return isUsageType(usage, Usage::Img); }

        //获取详细描述
        static QString desc(const DspStgy stgy);
        static QString desc(const DspOrder order);
        static QString desc(const Usage usage);
        static QString desc(const keyHash_t keyHash);
        static QString desc(const Qt::Key key); //TODO: QEnum的转换无法满足预期, 可能得自己写个转换表
        static QString desc(int) = delete;  //防止隐式转换和非预期的调用

    private:
        CCfgHelper() = delete;
        CCfgHelper(CCfgHelper&) = delete;
        CCfgHelper(CCfgHelper&&) = delete;
        ~CCfgHelper() = delete;
        static int getMod(const QKeyEvent* ev);
        static bool isUsageType(Usage usage, Usage usageType);
    };
}

#endif //!CFG_HELPER_H