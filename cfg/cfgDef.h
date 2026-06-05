#ifndef CFG_DEF_H
#define CFG_DEF_H

#define NO_MODIFILER    0x0
#define MOD_CTRL        0x1
#define MOD_ALT         0x2
#define MOD_SHIFT       0x4

class QKeyEvent;
namespace CFG
{
    typedef long long keyHash_t;

    //显示策略
    enum class DspStgy
    {
        RealSize = 0,
        FitWin,
    };

    //显示顺序
    enum class DspOrder
    {
        ByName = 0,
        ByTime,
    };

    //键鼠操作对应的功能/用途
    enum class Usage
    {
        Invalid = 0,

        //窗口操作
        Win = 0x100,
        Help,

        //文件操作
        File = 0x200,
        Copy,
        Del,
        Next_Img,
        Prev_Img,

        //图片观看操作
        Img = 0x300,
        //缩放
        ZoomIn,
        ZoomOut,
        ZoomReset,
        //移动视线
        SightUp,
        SightDown,
        SightLeft,
        SightRight,
        SightUp_1px,
        SightDown_1px,
        SightLeft_1px,
        SightRight_1px,
    };

    class CCfgHelper
    {
    public:
        //将按键及其功能键合并计算为一个64位长整形
        static keyHash_t getKeyHash(int key, int modifiler = NO_MODIFILER);
        static keyHash_t getKeyHash(const QKeyEvent* ev);

        //判断用途类型
        static bool isWinUsage(Usage usage) { return isUsageType(usage, Usage::Win); }
        static bool isFileUsage(Usage usage) { return isUsageType(usage, Usage::File); }
        static bool isImgUsage(Usage usage) { return isUsageType(usage, Usage::Img); }

        //获取详细描述
        //static const char* const desc(const DspStgy stgy);

    private:
        CCfgHelper() = delete;
        CCfgHelper(CCfgHelper&) = delete;
        CCfgHelper(CCfgHelper&&) = delete;
        ~CCfgHelper() = delete;
        static bool isUsageType(Usage usage, Usage usageType);
    };
}
#endif  //!CFG_DEF_H