#ifndef PREFERENCESDLG_H
#define PREFERENCESDLG_H

#include <QDialog>
#include <QSettings>

#include "util.h"

namespace Ui {
class PreferencesDlg;
}

class PreferencesDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit PreferencesDlg(QWidget *parent = 0);
    ~PreferencesDlg();
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::PreferencesDlg *ui;
};

#endif // PREFERENCESDLG_H
