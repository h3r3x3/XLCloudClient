/*
 *  CloudClient - A Qt cloud client for lixian.vip.xunlei.com
 *  Copyright (C) 2012 by Aaron Lewis <the.warl0ck.1989@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
