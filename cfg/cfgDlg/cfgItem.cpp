#include "cfgItem.h"
#include <QStandardItem>
#include <QPainter>

namespace CFG {
    QStandardItem* CFG::tag_cfgItem::toItem(void)
    {
        //配置项名称以图片方式显示
        static QPixmap pxTemplate(200, 50);
        static QPainter painter;
        QPixmap cfgNamePixmap = pxTemplate.copy();
        painter.begin(&cfgNamePixmap);
        painter.drawText(cfgNamePixmap.rect(), Qt::AlignCenter, name);
        painter.end();

        auto* item = new QStandardItem;
        item->setData(name, Role::CfgNameRole);
        item->setData(cfgNamePixmap, Qt::DecorationRole);   //配置项名称-图标
        item->setData(valType, Role::CfgValTypeRole);
        item->setData(val, Role::CfgValRole);
        item->setData(valDesc, Role::CfgDescRole);
        item->setData(val, Role::OldCfgValRole);
        item->setData(valAddr, Role::CfgValAddrRole);
        item->setData(keyUsageDictAddr, Role::KeyUsageDictAddrRole);
        item->setData(editor, Role::EditorRole);
        item->setData(cbItems, Role::ComboBoxItemsRole);
        item->setData(cbVals, Role::ComboBoxValsRole);
        return item;
    }

    tag_cfgItem tag_cfgItem::fromItem(const QModelIndex& index)
    {
        return tag_cfgItem()
            .set_name(index.data(Role::CfgNameRole).toString())
            .set_valType(index.data(Role::CfgValTypeRole).toInt())
            .set_val(index.data(Role::CfgValRole))
            .set_valDesc(index.data(Role::CfgDescRole).toString())
            .set_oldVal(index.data(Role::OldCfgValRole))
            .set_valAddr(index.data(Role::CfgValAddrRole).toULongLong())
            .set_keyUsageDictAddr(index.data(Role::KeyUsageDictAddrRole).toULongLong())
            .set_editor(index.data(Role::EditorRole).toInt())
            .set_cbItems(index.data(Role::ComboBoxItemsRole).toStringList())
            .set_cbVals(index.data(Role::ComboBoxValsRole).toList());
    }

}   //!namespace CFG