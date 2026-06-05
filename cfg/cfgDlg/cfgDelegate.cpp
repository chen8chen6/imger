#include "cfgDelegate.h"
#include <QComboBox>
#include <QDebug>
#include "cfgItem.h"

namespace CFG {
    QWidget* CCfgDele::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        QWidget* res = nullptr;
        switch (getEditorType(index))
        {
        case CFG::InvalidEditor:
            break;
        case CFG::IntEditor_comboBox:
            res = createEditor(parent, TCfgItem::fromItem(index));
            break;
        case CFG::IntEditor_line:
            break;
        case CFG::KeyEditor:
            break;
        default:
            break;
        }

        return res;
    }

    void CCfgDele::setEditorData(QWidget* editor, const QModelIndex& index) const
    {
        switch (getEditorType(index))
        {
        case CFG::InvalidEditor:
            break;
        case CFG::IntEditor_comboBox:
        {
            QComboBox* cb = static_cast<QComboBox*>(editor);
            int idx = cb->findData(index.data(Role::CfgValRole));
            qDebug() << "find " << index.data(Role::CfgValRole).toInt() << "at idx" << idx;
            cb->setCurrentIndex(std::max(0, idx));
            break;
        }
        case CFG::IntEditor_line:
            break;
        case CFG::KeyEditor:
            break;
        default:
            break;
        }
        return;
    }

    void CCfgDele::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
    {
        switch (getEditorType(index))
        {
        case CFG::InvalidEditor:
            break;
        case CFG::IntEditor_comboBox:
        {
            /**
            * 调用setData()会触发setEditor()引起数据变动,
            * 导致currentText()和currentData取到非预期值,
            * 因此此处需要保存一份副本用于更新
            **/
            QComboBox* cb = static_cast<QComboBox*>(editor);
            QVariant newDesc = cb->currentText();
            QVariant newCfgVal = cb->currentData();
            model->setData(index, newDesc, Role::CfgDescRole);
            model->setData(index, newCfgVal, Role::CfgValRole);
            break;
        }
        case CFG::IntEditor_line:
            break;
        case CFG::KeyEditor:
            break;
        default:
            break;
        }

        return;
    }

    int CCfgDele::getEditorType(const QModelIndex& index) const
    {
        QVariant varType = index.data(Role::EditorRole);
        return (varType.canConvert<int>()) ? varType.toInt() : Editor::InvalidEditor;
    }

    int CCfgDele::getCfgValType(const QModelIndex& index) const
    {
        QVariant varType = index.data(Role::CfgValTypeRole);
        return (varType.canConvert<int>()) ? varType.toInt() : CfgValType::InvalidValType;
    }

    QWidget* CCfgDele::createEditor(QWidget* parent, const TCfgItem& cfgItem) const
    {
        //添加选项
        QComboBox* cb = new QComboBox(parent);
        const int cnt = std::min(cfgItem.cbDescs.count(), cfgItem.cbVals.count());
        for (int i = 0; i < cnt; ++i)
            cb->addItem(cfgItem.cbDescs.at(i), cfgItem.cbVals.at(i));
        return cb;
    }

}   //!namespace CFG