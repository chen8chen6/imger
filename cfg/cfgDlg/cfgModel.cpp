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
                CCfgHelper::desc(static_cast<DspStgy>(*pStgy)), pStgy,
                { CCfgHelper::desc(DspStgy::RealSize), CCfgHelper::desc(DspStgy::FitWin) },
                { static_cast<int>(DspStgy::RealSize) ,static_cast<int>(DspStgy::FitWin) });
            imgView->appendRow(dspStgy);
        }

        //dspOrder
        {
            int* pOrder = &(m_cfgBak.imgView.dspOrder);
            auto* dspOrder = createComboBoxItem(
                QString::fromUtf8("显示顺序"),
                CCfgHelper::desc(static_cast<DspOrder>(*pOrder)), pOrder,
                { CCfgHelper::desc(DspOrder::ByName), CCfgHelper::desc(DspOrder::ByTime) },
                { static_cast<int>(DspOrder::ByName), static_cast<int>(DspOrder::ByTime) });
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
        if (CfgValType::IntValType == cfgItem.valType)
        {
            int newVal = cfgItem.val.toInt();
            int* valAddr = reinterpret_cast<int*>(cfgItem.valAddr);
            //qDebug() << "varAddr = " << QString::number(cfgItem.valAddr, 16);
            qDebug() << index.data(Role::CfgNameRole).toString() << *valAddr << "->" << newVal;
            *valAddr = newVal;

            setData(index, newVal, Role::OldCfgValRole);//modelItem 里的 oldVal也要更新
        }
        else if (CfgValType::KeyHashValType == cfgItem.valType)
        {
            keyHash_t newVal = cfgItem.val.toULongLong();
            keyHash_t oldVal = cfgItem.oldVal.toULongLong();
            keyUsageDict_t* pDict =
                reinterpret_cast<keyUsageDict_t*>(cfgItem.keyUsageDictAddr);
            Usage usage = pDict->at(oldVal);

            qDebug() << QStringLiteral("%1 : %2 -> %3")
                .arg(QString::number(static_cast<int>(usage), 16),
                    QString::number(oldVal, 16), QString::number(newVal, 16));

            pDict->erase(oldVal);
            pDict->emplace(newVal, usage);

            setData(index, newVal, Role::OldCfgValRole);//modelItem 里的 oldVal也要更新
        }


        return;
    }

    QStandardItem* CCfgItemModel::createComboBoxItem(QString&& name, QString&& valDesc, int* bindingValAddr, QStringList&& cbItems, QVariantList&& cbVals) const
    {
        return TCfgItem()
            .set_name(std::move(name))
            .set_valType(CfgValType::IntValType)
            .set_val(*bindingValAddr)
            .set_valDesc(std::move(valDesc))
            .set_oldVal(*bindingValAddr)
            .set_valAddr(reinterpret_cast<unsigned long long>(bindingValAddr))
            .set_editor(Editor::ComboBoxEditor)
            .set_cbItems(std::move(cbItems))
            .set_cbVals(std::move(cbVals))
            .toItem();
    }

    QStandardItem* CCfgItemModel::createKeyUsageItem(keyHash_t keyHash, Usage usage, keyUsageDict_t* pDict) const
    {
        return TCfgItem()
            .set_name(CCfgHelper::desc(usage))
            .set_valType(CfgValType::KeyHashValType)
            .set_val(keyHash)
            .set_valDesc(CCfgHelper::desc(keyHash))
            .set_oldVal(keyHash)
            .set_keyUsageDictAddr(reinterpret_cast<unsigned long long>(pDict))
            .set_editor(Editor::KeyEditor)
            .toItem();
    }

} //!namespace CFG