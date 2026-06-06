#ifndef CFG_MGR_H
#define CFG_MGR_H

#include <map>
#include "cfgDef.h"

class QDomDocument;
class QDomElement;
namespace CFG
{
    class CCfgMgr
    {
    public:
        typedef struct tag_imgViewCfg
        {
            int dspStgy = 0;    //0: RealSize; 1: FitWin
            int dspOrder = 0;   //0: SortByName, 1: SortByTime
            std::map<keyHash_t, Usage> keyUsageDict;    //使用keyHash索引找对应的usage
        } TImgViewCfg;
        typedef struct tag_cfg { TImgViewCfg imgView; } TCfg;   //TODO:可以放到cfgDef里去? 如果只有MVC用到, 甚至可以放到cfgItem里去

    public:
        static CCfgMgr* getSingleton(void);
        int load(void);
        int save(void) const;
        void reset(void);
        void update(const TCfg& other) { m_cfg = other; };
        void update(TCfg&& other) { std::swap(m_cfg, other); };
        const TCfg* getCfg(void) const { return &m_cfg; }

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
}   //! namespace CFG


#endif  //!CFG_MGR_H