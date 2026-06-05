#include "cfgModel.h"
#include <QPainter>
#include <QDebug>
#include "cfgMgr.h"
#include "cfgItem.h"

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
            auto* dspStgy = TCfgItem()
                .seteditor(Editor::IntEditor_comboBox)
                .setcfgName(QString::fromUtf8("显示策略"))
                .setcfgValType(CfgValType::IntValType)
                .setcfgValAddr(reinterpret_cast<unsigned long long>(&(m_cfgBak.imgView.dspStgy)))
                .setcfgVal(m_cfgBak.imgView.dspStgy)
                .setcfgDesc((static_cast<int>(DspStgy::RealSize) == m_cfgBak.imgView.dspStgy) ? QString::fromUtf8("原始大小") : QString::fromUtf8("适应窗口"))
                .setcbDescs({ QString::fromUtf8("原始大小"), QString::fromUtf8("适应窗口") })
                .setcbVals({ static_cast<int>(DspStgy::RealSize) , static_cast<int>(DspStgy::FitWin) })
                .toItem();
            imgView->appendRow(dspStgy);
        }

        //dspOrder
        {
            auto* dspOrder = TCfgItem()
                .seteditor(Editor::IntEditor_comboBox)
                .setcfgName(QString::fromUtf8("显示顺序"))
                .setcfgValType(CfgValType::IntValType)
                .setcfgValAddr(reinterpret_cast<unsigned long long>(&(m_cfgBak.imgView.dspOrder)))
                .setcfgVal(m_cfgBak.imgView.dspOrder)
                .setcfgDesc((static_cast<int>(DspOrder::ByName) == m_cfgBak.imgView.dspOrder) ? QString::fromUtf8("按文件名") : QString::fromUtf8("按时间"))
                .setcbDescs({ QString::fromUtf8("按文件名"), QString::fromUtf8("按时间") })
                .setcbVals({ static_cast<int>(DspOrder::ByName) , static_cast<int>(DspOrder::ByTime) })
                .toItem();
            imgView->appendRow(dspOrder);
        }

#if 0
        //keyBinding
        {
            auto* keyBinding = new QStandardItem("key bingding");
            keyBinding->setEditable(false);
            imgView->appendRow(keyBinding);

            auto* keyUsage = new QStandardItem;
            QPixmap tag(200, 50);
            QPainter painter;
            painter.begin(&tag);
            painter.drawText(tag.rect(), Qt::AlignCenter, "next img");
            painter.end();
            keyUsage->setData(QIcon(tag), Qt::DecorationRole);
            keyUsage->setData(QString::number(m_cfgBak.imgView.keyUsageDict.begin()->first, 16), Qt::EditRole);
            keyBinding->appendRow(keyUsage);
        }
#endif
        return;
    }

    void CCfgItemModel::updateCfg(const QModelIndex& index)
    {
        TCfgItem cfgItem = TCfgItem::fromItem(index);
        if (CfgValType::IntValType == cfgItem.cfgValType)
        {
            int newVal = cfgItem.cfgVal.toInt();
            int* valAddr = reinterpret_cast<int*>(cfgItem.cfgValAddr);
            qDebug() << "varAddr = " << QString::number(cfgItem.cfgValAddr, 16);
            qDebug() << *valAddr << "->" << newVal;
            *valAddr = newVal;
        }

        return;
    }

} //!namespace CFG