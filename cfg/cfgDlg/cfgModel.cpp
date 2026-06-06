#include "cfgModel.h"
#include <QPainter>
#include <QDebug>
#include "cfgMgr.h"
#include "cfgItem.h"    //TCfgItem
#include "cfgHelper.h"  //desc()

namespace CFG {
    void CCfgItemModel::updateCfgVal(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
    {
        qDebug() << __FUNCTION__;
        //TODO:设置为一次最多选择一个项目, 避免多个项目被同时修改
        bool hasCfgValRole = (roles.end() != std::find(roles.begin(), roles.end(), Role::CfgValRole));
        if (roles.empty()   //roles为空表示所有属性都有可能变更
            || hasCfgValRole)
        {
            updateCfg(topLeft);
        }
        return;
    }

    void CCfgItemModel::init(CCfgMgr* cfgMgr)
    {
        m_cfgMgr = cfgMgr;
        m_cfgBak = *(m_cfgMgr->getCfg());
        buildModel();
        connect(this, &CCfgItemModel::dataChanged, this, &CCfgItemModel::updateCfgVal);
        return;
    }

    void CCfgItemModel::buildModel(void)
    {
        /*
        * root
        *   |-- imgView
        *     |--dspStgy
        *     |--dspOrder
        *     |--keyBinding
        *        |--keyUsage
        *        |--keyUsage
        */

        auto* root = invisibleRootItem();
        auto* imgView = new QStandardItem("ImgView");
        imgView->setEditable(false);
        root->appendRow(imgView);

        //dspStgy
        {
            int* pStgy = &(m_cfgBak.imgView.dspStgy);
            auto* dspStgy = createComboBoxItem(
                QString::fromUtf8("显示策略"),
                CCfgHelper::desc(static_cast<DspStgy::Stgy>(*pStgy)), pStgy,
                { CCfgHelper::desc(DspStgy::RealSize), CCfgHelper::desc(DspStgy::FitWin) },
                { DspStgy::RealSize , DspStgy::FitWin });
            imgView->appendRow(dspStgy);
        }

        //dspOrder
        {
            int* pOrder = &(m_cfgBak.imgView.dspOrder);
            auto* dspOrder = createComboBoxItem(
                QString::fromUtf8("显示顺序"),
                CCfgHelper::desc(static_cast<DspOrder::Order>(*pOrder)), pOrder,
                { CCfgHelper::desc(DspOrder::ByName), CCfgHelper::desc(DspOrder::ByTime) },
                { DspOrder::ByName, DspOrder::ByTime });
            imgView->appendRow(dspOrder);
        }

        //keyBinding
        {
            auto* keyBinding = new QStandardItem("key bingding");
            keyBinding->setEditable(false);
            imgView->appendRow(keyBinding);

            auto& keyUsageDict = m_cfgBak.imgView.keyUsageDict;
            for (const auto& keyUsageItem : keyUsageDict)
            {
                auto* keyUsage = createKeyUsageItem(keyUsageItem.first, keyUsageItem.second, &keyUsageDict);
                keyBinding->appendRow(keyUsage);
            }
            //keyBinding->sortChildren(0);    //TODO: 优化显示顺序
        }
        return;
    }

    void CCfgItemModel::updateCfg(const QModelIndex& index)
    {
        TCfgItem cfgItem = TCfgItem::fromItem(index);
        if (CfgValType::IntValType == cfgItem.cfgValType)
        {
            int newVal = cfgItem.cfgVal.toInt();
            int* valAddr = reinterpret_cast<int*>(cfgItem.cfgValAddr);
            //qDebug() << "varAddr = " << QString::number(cfgItem.cfgValAddr, 16);
            qDebug() << index.data(Role::CfgNameRole).toString() << *valAddr << "->" << newVal;
            *valAddr = newVal;

            setData(index, newVal, Role::OldCfgValRole);//modelItem 里的 oldCfgVal也要更新
        }
        else if (CfgValType::KeyHashValType == cfgItem.cfgValType)
        {
            keyHash_t newVal = cfgItem.cfgVal.toULongLong();
            keyHash_t oldVal = cfgItem.oldCfgVal.toULongLong();
            std::map<keyHash_t, Usage>* pDict =
                reinterpret_cast<std::map<keyHash_t, Usage>*>(cfgItem.keyUsageDictAddr);
            CFG::Usage usage = pDict->at(oldVal);

            qDebug() << QStringLiteral("%1 : %2 -> %3")
                .arg(QString::number(static_cast<int>(usage), 16),
                    QString::number(oldVal, 16), QString::number(newVal, 16));

            pDict->erase(oldVal);
            pDict->emplace(newVal, usage);

            setData(index, newVal, Role::OldCfgValRole);//modelItem 里的 oldCfgVal也要更新
        }


        return;
    }

    QStandardItem* CCfgItemModel::createComboBoxItem(QString&& cfgName, QString&& cfgDesc, int* bindingValAddr, QStringList&& cbDescs, QVariantList&& cbVals) const
    {
        return TCfgItem()
            .seteditor(Editor::IntEditor_comboBox)
            .setcfgValType(CfgValType::IntValType)
            .setcfgName(std::move(cfgName))
            .setcfgValAddr(reinterpret_cast<unsigned long long>(bindingValAddr))
            .setcfgVal(*bindingValAddr)
            .setoldCfgVal(*bindingValAddr)
            .setcfgDesc(std::move(cfgDesc))
            .setcbDescs(std::move(cbDescs))
            .setcbVals(std::move(cbVals))
            .toItem();
    }

    QStandardItem* CCfgItemModel::createKeyUsageItem(keyHash_t keyHash, Usage usage, std::map<keyHash_t, Usage>* pDict) const
    {
        return TCfgItem()
            .seteditor(Editor::KeyEditor)
            .setcfgName(CCfgHelper::desc(usage))
            .setcfgValType(CfgValType::KeyHashValType)
            .setcfgVal(keyHash)
            .setoldCfgVal(keyHash)
            .setcfgDesc(CCfgHelper::desc(keyHash))
            .setkeyUsageDictAddr(reinterpret_cast<unsigned long long>(pDict))
            .toItem();
    }

} //!namespace CFG