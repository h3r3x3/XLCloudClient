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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tcore(new ThunderCore (this)),
    tpanel(new ThunderPanel (this)),
    vpanel(new VideoPanel (this)),
    lpanel(new LogView (this)),
    bpanel(new Browser (this)),
    transf0r(new Transf0r (this))
{
    ui->setupUi(this);
    setWindowTitle (tr("%1 $VER %2").arg(QApplication::applicationName())
                    .arg(QApplication::applicationVersion()));
    vpanel->setStatusBar(ui->statusBar);

    ui->tabWidget->addTab(tpanel, "Cloud");
    ui->tabWidget->addTab(vpanel, "Video");
    ui->tabWidget->addTab(bpanel, "Browser");
    ui->tabWidget->addTab(lpanel, "Log");
    ui->tabWidget->addTab(transf0r, "Transf0r");

    connect (tcore, SIGNAL(StatusChanged(ThunderCore::ChangeType)),
             SLOT(slotStatusChanged(ThunderCore::ChangeType)));
    connect (tcore, SIGNAL(error(QString,ThunderCore::ErrorCategory)),
             lpanel, SLOT(logReceived(QString,ThunderCore::ErrorCategory)));

    connect (tpanel, SIGNAL(doThisLink(Thunder::RemoteTask,ThunderPanel::RequestType)),
             SLOT(slotRequestReceived(Thunder::RemoteTask,ThunderPanel::RequestType)));
    connect (tpanel, SIGNAL(doIndirectRequest(ThunderPanel::IndirectRequestType)),
             SLOT(slotIndirectRequestReceived(ThunderPanel::IndirectRequestType)));

    // WILL BE REMOVED LATER!
    connect (tcore, SIGNAL(error(QString,ThunderCore::ErrorCategory)),
             SLOT(slotError(QString,ThunderCore::ErrorCategory)));

    ///
    {
        QSettings settings;
        settings.beginGroup("UI");
        restoreState( settings.value("State").toByteArray());
        restoreGeometry( settings.value("Geometry").toByteArray() );
    }

    login ();
}

MainWindow::~MainWindow()
{
    QSettings sets;
    sets.beginGroup("UI");
    sets.setValue("State", saveState());
    sets.setValue("Geometry", saveGeometry());
    sets.endGroup();

    delete ui;
}

void MainWindow::login()
{
    QSettings settings;
    settings.beginGroup("General");

    const QString & user = settings.value("User").toString();
    const QString & credential = settings.value("Credential").toString();

    if (! user.isEmpty() && ! credential.isEmpty())
        tcore->login(user, credential);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->modifiers())
    {
    case Qt::CTRL:
        break;
    case Qt::ALT:
        if (e->key() >= Qt::Key_1 && e->key() <= Qt::Key_9)
        {
            int delta = e->key() - Qt::Key_1;
            if (delta < ui->tabWidget->count())
                ui->tabWidget->setCurrentIndex(delta);
        }

        break;
    }
}

void MainWindow::slotRequestReceived(const Thunder::RemoteTask &task,
                                     ThunderPanel::RequestType type)
{
    switch (type)
    {
    case ThunderPanel::Preview:
        vpanel->play(task.url);
        ui->tabWidget->setCurrentIndex(1);
        break;
    case ThunderPanel::Download:
        transf0r->addCloudTask(task);
        break;
    default:
        Q_ASSERT(false);
        break;
    }

}

bool MainWindow::question(const QString &body, const QString &title)
{
    return QMessageBox::No == QMessageBox::question(this, title, body, QMessageBox::Yes,
                                                    QMessageBox::No);
}

void MainWindow::slotIndirectRequestReceived(ThunderPanel::IndirectRequestType type)
{
    if (type == ThunderPanel::RemoveTasks)
    {
        const QStringList & ids = tpanel->getSelectedTaskIDs();

        if (question ( tr("Remove selected %1 items?").arg(ids.size())) )
            return;

        tcore->removeCloudTasks(ids);

    } else if (type == ThunderPanel::AddTask) {

    }
}

void MainWindow::slotStatusChanged(ThunderCore::ChangeType type)
{
    switch (type)
    {
    case ThunderCore::LoginChanged:
        break;
    case ThunderCore::TaskChanged:
        tpanel->setCloudTasks(tcore->getCloudTasks());
        break;
    case ThunderCore::CapchaReady:
    {
        SayCapcha *sayCapcha = new SayCapcha (this);
        sayCapcha->setImage(tcore->getCapchaCode());
        connect (sayCapcha, SIGNAL(capchaReady(QString)),
                 tcore, SLOT(setCapcha(QString)));

        sayCapcha->exec();
    }
        break;
    default:
        Q_ASSERT(false);
        break;
    }
}

void MainWindow::slotError(const QString &body, ThunderCore::ErrorCategory category)
{
    qDebug() << category << body;
}

void MainWindow::on_actionAboutAuthor_triggered()
{
    QMessageBox::about(this, QApplication::applicationName(), tr("Xunlei (迅雷) VIP Cloud client<br/><br/>Written by <b>Aaron Lewis</b>"
                                                                 " (the.warl0ck.1989@gmail.com)"));
}

void MainWindow::slotSettingsChanged ()
{
    // BUG:   signal emitted prior to settings storage
    // Trick: delayed function calls
    QTimer::singleShot(1000, this, SLOT(login()));
}

void MainWindow::on_actionPreferences_triggered()
{
    PreferencesDlg *dlg = new PreferencesDlg (this);
    connect (dlg, SIGNAL(accepted()), SLOT(slotSettingsChanged()));
    dlg->exec();
}

void MainWindow::on_actionQ_uit_triggered()
{
    close ();
}

void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::on_actionNewTask_triggered()
{
    AddCloudTask *act = new AddCloudTask (tcore, this);
    act->exec();
}
