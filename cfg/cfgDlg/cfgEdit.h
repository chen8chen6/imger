#ifndef CFG_EDIT_H
#define CFG_EDIT_H

#include <QLineEdit>
#include "cfgDef.h" //keyHash_t

class QKeyEvent;
namespace CFG {

    class CKeyEdit : public QLineEdit
    {
    public:
        CKeyEdit(const keyUsageDict_t* pDict, QWidget* parent = nullptr);
        ~CKeyEdit(void) override;

        void keyPressEvent(QKeyEvent* ev) override;
        void keyReleaseEvent(QKeyEvent* ev) override;

        void setKeyHash(keyHash_t other) { m_keyHash = other; }
        keyHash_t getKeyHash(void) { return m_keyHash; }

        bool isEditFinished(void) { return m_isEditFinished; }

    private:
        bool isKeyUsed(keyHash_t keyHash) const;
        keyHash_t m_keyHash = 0;
        const keyUsageDict_t* const m_keyUsed = nullptr;
        bool m_isEditFinished = false;
    };




}//!namespace CFG

#endif //!CFG_EDIT_H