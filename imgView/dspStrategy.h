#ifndef CDSPSTRATEGY_H
#define CDSPSTRATEGY_H

class QPixmap;

//图片显示策略
class CDspStrategy
{
public:
    CDspStrategy() {}
    virtual ~CDspStrategy();
    virtual int process(QPixmap* img) = 0;
};

//缩放至指定大小(保持高宽比)
class CZoomToSize : public CDspStrategy
{
public:
    CZoomToSize(int width, int height);
    int process(QPixmap* img) override;

private:
    const int m_width = 0;
    const int m_height = 0;
    const double m_whRatio = 0.0;  //宽高比
};

//实际大小
class CRealSize : public CDspStrategy
{
public:
    int process(QPixmap* img) override { return 0; }
};

#endif // CDSPSTRATEGY_H
