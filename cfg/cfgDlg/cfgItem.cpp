#include "cfgItem.h"
#include <QStandardItem>
#include <QPainter>

namespace CFG {
    QStandardItem* CFG::tag_cfgItem::toItem(void)
    {
        static QPixmap pxTemplate(200, 50);
        static QPainter painter;

        //配置项名称以图片方式显示
        auto* item = new QStandardItem;
        QPixmap cfgNamePixmap = pxTemplate.copy();
        painter.begin(&cfgNamePixmap);
        painter.drawText(cfgNamePixmap.rect(), Qt::AlignCenter, cfgName);
        painter.end();

        item->setData(editor, Role::EditorRole);    //编辑器类型
        item->setData(cfgName, Role::CfgNameRole);  //配置项名称
        item->setData(cfgNamePixmap, Qt::DecorationRole);   //配置项名称-图标
        item->setData(cfgDesc, Qt::DisplayRole);    //当前配置值的解释
        item->setData(cfgValType, Role::CfgValTypeRole);
        item->setData(cfgValAddr, Role::CfgValAddrRole);
        item->setData(cfgVal, Role::CfgValRole);    //当前配置值
        item->setData(cbDescs, Role::ComboBoxItemsRole);    //comboBox的显示项目
        item->setData(cbVals, Role::ComboBoxValsRole);      //comboBox显示项目对应的值

        return item;
    }

    tag_cfgItem tag_cfgItem::fromItem(const QModelIndex& index)
    {
        return tag_cfgItem()
            .seteditor(index.data(Role::EditorRole).toInt())
            .setcfgName(index.data(Role::CfgNameRole).toString())
            .setcfgDesc(index.data(Role::CfgDescRole).toString())
            .setcfgValType(index.data(Role::CfgValTypeRole).toInt())
            .setcfgValAddr(index.data(Role::CfgValAddrRole).toULongLong())
            .setcfgVal(index.data(Role::CfgValRole))
            .setcbDescs(index.data(Role::ComboBoxItemsRole).toStringList())
            .setcbVals(index.data(Role::ComboBoxValsRole).toList());
    }

}   //!namespace CFG