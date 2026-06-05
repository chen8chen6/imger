#ifndef CFG_MODEL_H
#define CFG_MODEL_H

#include <QStandardItemModel>

#if 1
#include <QPersistentModelIndex>
#include <QMap>
#include "cfgMgr.h"
#endif


namespace CFG {

    class CCfgMgr;
    struct tag_cfgItem;
    class CCfgItemModel : public QStandardItemModel
    {
        Q_OBJECT

    public slots:
        void updateCfgVal(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);

    public:
        CCfgItemModel(QObject* parent = nullptr) : QStandardItemModel(parent) {}
        ~CCfgItemModel(void) override {}
        void init(CCfgMgr* cfgMgr);
        CCfgMgr::TCfg getCfg(void) { return m_cfgBak; }

    private:
        void buildModel(void);
        void updateCfg(const QModelIndex& index);

    private:
        CCfgMgr* m_cfgMgr = nullptr;
        CCfgMgr::TCfg m_cfgBak;
    };
}

#endif // CFG_MODEL_H
