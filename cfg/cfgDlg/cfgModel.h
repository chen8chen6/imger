#ifndef CFG_MODEL_H
#define CFG_MODEL_H

#include <QStandardItemModel>
#include "cfgDef.h" //keyHash_t, Usage, TCfg

namespace CFG {
    class CCfgMgr;
    class CCfgItemModel : public QStandardItemModel
    {
        Q_OBJECT

    public slots:
        void updateCfgVal(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);

    public:
        CCfgItemModel(QObject* parent = nullptr) : QStandardItemModel(parent) {}
        ~CCfgItemModel(void) override {}
        void init(CCfgMgr* cfgMgr);
        TCfg getCfg(void) { return m_cfgBak; }

    private:
        void buildModel(void);
        void updateCfg(const QModelIndex& index);
        QStandardItem* createComboBoxItem(QString&& name, QString&& valDesc, int* bindingValAddr,
            QStringList&& cbItems, QVariantList&& cbVals) const;
        QStandardItem* createKeyUsageItem(keyHash_t keyHash, Usage usage, keyUsageDict_t* pDict) const;

    private:
        CCfgMgr* m_cfgMgr = nullptr;
        TCfg m_cfgBak; //TODO:使用指针
    };
}

#endif // CFG_MODEL_H
