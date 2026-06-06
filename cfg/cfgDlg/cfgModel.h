#ifndef CFG_MODEL_H
#define CFG_MODEL_H

#include <QStandardItemModel>
#include "cfgMgr.h" //CCfgMgr::TCfg

#if 0
#include <QPersistentModelIndex>
#include <QMap>
#endif


namespace CFG {

    //class CCfgMgr;
    struct tag_cfgItem;
    typedef keyHash_t;
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
        QStandardItem* createComboBoxItem(QString&& cfgName, QString&& cfgDesc, int* bindingValAddr,
            QStringList&& cbDescs, QVariantList&& cbVals) const;
        QStandardItem* createKeyUsageItem(keyHash_t keyHash, Usage usage, std::map<keyHash_t, Usage>* pDict) const;

    private:
        CCfgMgr* m_cfgMgr = nullptr;
        CCfgMgr::TCfg m_cfgBak; //TODO:使用指针
    };
}

#endif // CFG_MODEL_H
