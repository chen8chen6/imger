#ifndef CFG_DELEGATE_H
#define CFG_DELEGATE_H

#include <QStyledItemDelegate>

namespace CFG {
    struct tag_cfgItem;
    class CCfgDele : public QStyledItemDelegate
    {
        Q_OBJECT
    public:
        CCfgDele(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}
        ~CCfgDele(void) override {}

        QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
        void setEditorData(QWidget* editor, const QModelIndex& index) const override;
        void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;

    private:
        int getEditorType(const QModelIndex& index) const;
        int getCfgValType(const QModelIndex& index) const;
        QWidget* createComboBoxEditor(QWidget* parent, const tag_cfgItem& cfgItem) const;
        QWidget* createKeyEditor(QWidget* parent, const tag_cfgItem& cfgItem) const;
    };

}   //!namespace CFG
#endif //!CFG_DELEGATE_H