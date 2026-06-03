#ifndef CFG_MGR_H
#define CFG_MGR_H

#include <map>
#include "cfgDef.h"

class QDomDocument;
class QDomElement;
class CCfgMgr
{
public:
	typedef struct tag_imgViewCfg
	{
		int dspStgy = 0;	//0: RealSize; 1: FitWin
		int dspOrder = 0;	//0: SortByName, 1: SortByTime
		std::map<keyHash_t, Usage> keyUsageDict;	//使用keyHash索引找对应的usage
	} TImgViewCfg;
	typedef struct tag_cfg { TImgViewCfg imgView; } TCfg;

public:
	static CCfgMgr* getSingleton(void);
	int load(void);
	int save(void) const;
	const TCfg* getCfg(void) const { return &m_cfg; }
	//int set(const CCFG *cfg);	//TODO: TCFG &&

private:
	static constexpr char CFG_FILE_NAME[] = "config.xml";

private:
	CCfgMgr() {};
	CCfgMgr(const CCfgMgr& cfg) = delete;
	CCfgMgr(const CCfgMgr&& cfg) = delete;
	virtual ~CCfgMgr() {};

	void buildCfg_imgView(QDomDocument& doc, QDomElement& root) const;
	int parseCfg_imgView(QDomDocument& doc, QDomElement& root);
	int saveFile(QDomDocument& doc) const;

	static void appendKeyUsage(QDomDocument& doc, QDomElement& parent,
		keyHash_t keyHash, Usage usage);

private:
	TCfg m_cfg;
};


#endif	//!CFG_MGR_H