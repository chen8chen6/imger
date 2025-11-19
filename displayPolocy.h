#ifndef CDISPLAYPOLICY_H
#define CDISPLAYPOLICY_H

class QPixmap;

//图片显示策略
class CDisplayPolicy
{
public:
    CDisplayPolicy() {};
    virtual ~CDisplayPolicy() {};

    virtual int process(QPixmap *img) = 0;
};

//扩展以适应屏幕
class CExpand2FitWin : public CDisplayPolicy
{
public:
    int process(QPixmap *img) override;
};

//实际大小
class CRealSize : public CDisplayPolicy
{
public:
    int process(QPixmap *img) override { return 0; }
};

#endif // CDISPLAYPOLICY_H
