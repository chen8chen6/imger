#ifndef CFG_ITEM_H
#define CFG_ITEM_H

#include <QString>
#include <QVariant>
#include <QList>

#define PROPERTY_WITH_SETTER(type, property) \
    type property; \
    tag_cfgItem & set##property (type && other) \
        {property = other; return *this; }

class QStandardItem;
namespace CFG {

    enum CfgValType
    {
        InvalidValType = 0,
        IntValType,
        KeyHashValType,
    };

    enum Role
    {
        CfgDescRole = 0, //Qt::DisplayRole,
        //SortRole = Qt::SortRole;
        UserRole = 256, //Qt::UserRole,
        CfgNameRole,

        //用于直接修改TCfg里的对应变量
        CfgValTypeRole,
        CfgValRole,
        OldCfgValRole,
        CfgValAddrRole,

        //TODO: keyHash单独存个role?
        //用于修改Keybinding, 直接存map地址, 因为map的节点地址是会失效的
        //KeyHashRole,
        KeyUsageDictAddrRole,

        //用于生成Editor
        EditorRole,
        ComboBoxItemsRole,
        ComboBoxValsRole,
    };

    enum Editor
    {
        InvalidEditor = 0,
        IntEditor_comboBox, //TODO: cbBox的val里都是QVariant, 倒没必要区分Int和String
        IntEditor_line,
        KeyEditor,
    };

    typedef struct tag_cfgItem
    {
        PROPERTY_WITH_SETTER(QString, cfgName);     //配置项名称
        PROPERTY_WITH_SETTER(QString, cfgDesc);     //当前配置值的解释
        PROPERTY_WITH_SETTER(int, cfgValType);      //配置值的数据类型
        PROPERTY_WITH_SETTER(QVariant, cfgVal);     //当前配置值
        PROPERTY_WITH_SETTER(QVariant, oldCfgVal);     //修改前的配置值

        //修改绑定的配置变量时使用
        PROPERTY_WITH_SETTER(unsigned long long, cfgValAddr);      //配置值的内存地址
        PROPERTY_WITH_SETTER(unsigned long long, keyUsageDictAddr);      //keyUsageDict的内存地址

        //生成editor时使用
        PROPERTY_WITH_SETTER(int, editor);          //编辑器类型
        PROPERTY_WITH_SETTER(QStringList, cbDescs); //comboBox的显示项目
        PROPERTY_WITH_SETTER(QList<QVariant>, cbVals);//comboBox显示项目对应的值

        static tag_cfgItem fromItem(const QModelIndex& index);
        QStandardItem* toItem(void);    //这个Item是new出来的, 没有移交给model的话, 需要手动delete


    } TCfgItem;

}

#endif  //!CFG_ITEM_H