#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QProxyModel>
#include <QKeyEvent>

#include "thundercore.h"
#include "thunderpanel.h"
#include "logview.h"
#include "preferencesdlg.h"
#include "videopanel.h"
#include "browser.h"
#include "addcloudtask.h"
#include "transf0r.h"
#include "saycapcha.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;

    ThunderCore *tcore;
    ThunderPanel *tpanel;
    VideoPanel *vpanel;
    LogView *lpanel;
    Browser *bpanel;
    Transf0r *transf0r;

protected:
    void keyPressEvent(QKeyEvent *e);

public slots:
    void slotError (const QString & body, ThunderCore::ErrorCategory category);
    void slotStatusChanged (ThunderCore::ChangeType type);

    void slotRequestReceived (const Thunder::RemoteTask & task,
                              ThunderPanel::RequestType type);
    void slotIndirectRequestReceived (ThunderPanel::IndirectRequestType type);

private:
    bool question (const QString & body, const QString & title = tr("Question"));

private slots:
    void on_actionAboutAuthor_triggered();
    void on_actionPreferences_triggered();
    void on_actionQ_uit_triggered();
    void on_actionAboutQt_triggered();
    void on_actionNewTask_triggered();
};

#endif // MAINWINDOW_H
