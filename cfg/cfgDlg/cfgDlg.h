#ifndef CFG_DLG_H
#define CFG_DLG_H

#include <QDialog>

namespace Ui {
    class CCfgDlg;
}

namespace CFG
{
    class CCfgMgr;
    class CCfgItemModel;
    struct tag_cfgItem;
    class CCfgDlg : public QDialog
    {
        Q_OBJECT

    public:
        explicit CCfgDlg(QWidget* parent = nullptr);
        ~CCfgDlg();
        int init(CFG::CCfgMgr* cfgMgr);
        int saveCfgFile(void);

    private:
        Ui::CCfgDlg* ui;
        CFG::CCfgMgr* m_cfgMgr = nullptr;
        CCfgItemModel* m_model = nullptr;
    };
}

#endif // CFGDLG_H
