#include "cfgEdit.h"
#include <QKeyEvent>
#include <QDebug>
#include "cfgHelper.h"  //getKeyHash(), desc()

namespace CFG {
    CKeyEdit::CKeyEdit(const keyUsageDict_t* pDict, QWidget* parent)
        : QLineEdit(parent), m_keyUsed(pDict)
    {
    }

    CKeyEdit::~CKeyEdit(void)
    {
    }

    void CFG::CKeyEdit::keyPressEvent(QKeyEvent* ev)
    {
        const keyHash_t newHash = CCfgHelper::getKeyHash(ev);
        const QString newDesc = CCfgHelper::desc(newHash);
        m_isEditFinished = false;

        do
        {
            if (CCfgHelper::isModKey(ev->key()))
            {
                //仅按下功能键, 未完成编辑
                setText(newDesc);
                break;
            }

            if (newHash != m_keyHash && isKeyUsed(newHash))
            {
                //检测到按键冲突
                static QString keyConflict = QString::fromUtf8("%1 已被分配给 %2");  //TODO: QStringLiteral + tr("")
                setText(keyConflict.arg(newDesc, CCfgHelper::desc(m_keyUsed->at(newHash))));
                break;;
            }

            //完成编辑, 取消焦点
            setText(newDesc);
            setKeyHash(newHash);
            m_isEditFinished = true;
            clearFocus();

        } while (0);

        return;
    }

    void CKeyEdit::keyReleaseEvent(QKeyEvent* ev)
    {
        if (CCfgHelper::isModKey(ev->key()))
            setText(CCfgHelper::desc(CCfgHelper::getKeyHash(ev)));
        return;
    }

    bool CKeyEdit::isKeyUsed(keyHash_t keyHash) const
    {
        return m_keyUsed->count(keyHash) > 0;
    }

} //!namespace CFG
