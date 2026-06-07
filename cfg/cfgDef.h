#ifndef CFG_DEF_H
#define CFG_DEF_H

#include <map>

namespace CFG
{
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
        Config,

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

    typedef long long keyHash_t;
    typedef std::map<keyHash_t, Usage> keyUsageDict_t;
    typedef struct tag_imgViewCfg
    {
        int dspStgy = 0;    //0: RealSize; 1: FitWin
        int dspOrder = 0;   //0: SortByName, 1: SortByTime
        keyUsageDict_t keyUsageDict;    //使用keyHash索引找对应的usage
    } TImgViewCfg;
    typedef struct tag_cfg { TImgViewCfg imgView; } TCfg;
}
#endif  //!CFG_DEF_H