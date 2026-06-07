#ifndef CFG_ITEM_H
#define CFG_ITEM_H

#include <QString>
#include <QVariant>
#include <QList>

#define PROPERTY_WITH_SETTER(type, property) \
    type property; \
    tag_cfgItem & set_##property (type && other) \
        {property = other; return *this; }

class QStandardItem;
namespace CFG {

    //TODO: 这个枚举目前看起来没使用场景, 可以考虑删除
    enum CfgValType
    {
        InvalidValType = 0,
        IntValType,
        KeyHashValType,
    };

    //仿照Qt::Role, 作用类似宏, 目的就是当int用, 不使用enum class
    enum Role
    {
        //用于生成ModelItem的基本项目
        CfgDescRole = 0,        //Qt::DisplayRole, //当前值的解释性描述
        CfgNameRole = 256 + 1,  //Qt::UserRole + 1, 配置项名称
        CfgValTypeRole,         //配置项使用的数据类型
        CfgValRole,             //配置项的当前值

        //用于修改配置项在内存中对应的变量
        OldCfgValRole,          //修改前的旧值
        CfgValAddrRole,         //配置值的内存地址
        KeyUsageDictAddrRole,   //用于修改按键绑定, 直接存map地址, 因为map的节点地址是会失效的

        //用于生成Editor
        EditorRole,             //编辑器类型
        ComboBoxItemsRole,      //comboBox的显示项目
        ComboBoxValsRole,       //comboBox显示项目对应的值
        //SortRole = Qt::SortRole;
    };

    enum Editor
    {
        InvalidEditor = 0,
        ComboBoxEditor,
        IntEditor_line,
        KeyEditor,
    };

    //抽象化的配置项, 对应一个modelItem
    //TODO: 能不能不经过TCfg, 直接从.xml文件转成model, 后续也在model里做搜索?
    typedef struct tag_cfgItem
    {
        PROPERTY_WITH_SETTER(QString, name);    //配置项名称
        PROPERTY_WITH_SETTER(int, valType);     //配置项使用的数据类型
        PROPERTY_WITH_SETTER(QVariant, val);    //配置项的当前值
        PROPERTY_WITH_SETTER(QString, valDesc); //当前值的解释性描述

        //用于modelItem更新时, 同步更新内存中对应的配置项变量
        PROPERTY_WITH_SETTER(QVariant, oldVal);                     //修改前的旧值
        PROPERTY_WITH_SETTER(unsigned long long, valAddr);          //配置值的内存地址
        PROPERTY_WITH_SETTER(unsigned long long, keyUsageDictAddr); //快捷键是以map(红黑树)的方式存在内存里的, 其节点地址随时会失效, 因此保存整个map, 配合存在oldVal里的keyHash找到对应项目

        //用于生成对应的editor
        PROPERTY_WITH_SETTER(int, editor);              //编辑器类型
        PROPERTY_WITH_SETTER(QStringList, cbItems);     //comboBox的显示项目
        PROPERTY_WITH_SETTER(QList<QVariant>, cbVals);  //comboBox显示项目对应的值

        static tag_cfgItem fromItem(const QModelIndex& index);
        QStandardItem* toItem(void);    //这个Item是new出来的, 没有移交给model的话, 需要手动delete
    } TCfgItem;

}

#endif  //!CFG_ITEM_H