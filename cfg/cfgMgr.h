#ifndef CFG_MGR_H
#define CFG_MGR_H

#include "cfgDef.h"

class QDomDocument;
class QDomElement;
namespace CFG
{
    class CCfgMgr
    {
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