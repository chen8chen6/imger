#ifndef CFG_EDIT_H
#define CFG_EDIT_H

#include <QLineEdit>
#include "cfgDef.h" //keyHash_t

class QKeyEvent;
namespace CFG {

    class CKeyEdit : public QLineEdit
    {
    public:
        CKeyEdit(QWidget* parent = nullptr);
        ~CKeyEdit(void) override;

        void keyPressEvent(QKeyEvent* ev) override;
        void keyReleaseEvent(QKeyEvent* ev) override;

        void setKeyHash(keyHash_t other) { m_keyHash = other; }
        keyHash_t getKeyHash(void) { return m_keyHash; }

    private:
        keyHash_t m_keyHash = 0;
    };




}//!namespace CFG

#endif //!CFG_EDIT_H