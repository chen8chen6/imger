#include "cfgDlg.h"
#include "ui_cfgDlg.h"
#include <QStandardItemModel>
#include <QPainter>
#include "cfgMgr.h"
#include "cfgDelegate.h"
#include "cfgModel.h"


namespace CFG {

    CCfgDlg::CCfgDlg(QWidget* parent) :
        QDialog(parent),
        ui(new Ui::CCfgDlg)
    {
        ui->setupUi(this);
    }

    CCfgDlg::~CCfgDlg()
    {
        delete ui;
    }

    int CCfgDlg::init(CCfgMgr* cfgMgr)
    {
        m_cfgMgr = cfgMgr;
        m_model = new CCfgItemModel(this);
        m_model->init(m_cfgMgr);
        ui->tvCfg->setModel(m_model);
        ui->tvCfg->setItemDelegate(new CCfgDele);
        connect(ui->btnSave, &QPushButton::clicked, this, &CCfgDlg::saveCfgFile);
        return 0;
    }

    int CCfgDlg::saveCfgFile(void)
    {
        ui->lbSt->setText(QString::fromUtf8("正在保存......"));
        m_cfgMgr->update(m_model->getCfg());
        int ret = m_cfgMgr->save();
        ui->lbSt->setText(0 == ret ? QString::fromUtf8("保存成功") : QString::fromUtf8("保存失败"));
        return ret;
    }

} //!namespace CFG