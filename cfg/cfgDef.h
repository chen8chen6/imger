#ifndef CFG_DEF_H
#define CFG_DEF_H

//TODO: 转成enum
#define NO_MODIFILER    0x0
#define MOD_CTRL        0x1
#define MOD_ALT         0x2
#define MOD_SHIFT       0x4

namespace CFG
{
    typedef long long keyHash_t;

    //显示策略
    //enum放在class里既有独立命名空间, 作为int使用时又不用static_cast
    class DspStgy
    {
    public:
        enum Stgy
        {
            RealSize = 0,
            FitWin,
        };
        //TODO: desc函数可以放在这里, enum也可以使用Q_ENUM
    };

    //显示顺序
    class DspOrder
    {
    public:
        enum Order
        {
            ByName = 0,
            ByTime,
        };
    };

    //键鼠操作对应的功能/用途
    //这个枚举使用时需要避免隐式转换, 因此使用enum class
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
        SightUp,    //TODO: rename to lookUp?
        SightDown,
        SightLeft,
        SightRight,
        SightUp_1px,
        SightDown_1px,
        SightLeft_1px,
        SightRight_1px,
    };

}
#endif  //!CFG_DEF_H