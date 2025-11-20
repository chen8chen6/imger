#include "imgMgr.h"
#include "cacheByNum.h"
#include "cacheAll.h"

CImgMgr::CImgMgr()
{

}

CImgMgr::~CImgMgr()
{

}

std::shared_ptr<CImgMgr> CImgMgrFac::create(int fileNum)
{
    if (fileNum > CCacheByNum::cacheSize())
        return std::shared_ptr<CImgMgr>(new CCacheByNum);
    else
        return std::shared_ptr<CImgMgr>(new CCacheAll);
}
