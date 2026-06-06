#include "cfgEdit.h"
#include <QKeyEvent>
#include <QDebug>
#include "cfgHelper.h"  //getKeyHash(), desc()

namespace CFG {
    CKeyEdit::CKeyEdit(QWidget* parent)
        : QLineEdit(parent)
    {
    }

    CKeyEdit::~CKeyEdit(void)
    {
    }

    void CFG::CKeyEdit::keyPressEvent(QKeyEvent* ev)
    {
        setText(CCfgHelper::desc(CCfgHelper::getKeyHash(ev)));
        if (!CCfgHelper::isModKey(ev->key()))
        {
            //完成编辑, 取消焦点
            setKeyHash(CCfgHelper::getKeyHash(ev));
            emit editingFinished();
            clearFocus();
        }

        return;
    }

    void CKeyEdit::keyReleaseEvent(QKeyEvent* ev)
    {
        if (CCfgHelper::isModKey(ev->key()))
        {
            setText(CCfgHelper::desc(CCfgHelper::getKeyHash(ev)));
        }
        return;
    }

} //!namespace CFG
