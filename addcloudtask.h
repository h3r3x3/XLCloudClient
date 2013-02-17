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

#ifndef ADDCLOUDTASK_H
#define ADDCLOUDTASK_H

#include <QDialog>
#include <QClipboard>
#include <QApplication>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QFileDialog>

#include "thundercore.h"
#include "simpleeditor.h"

namespace Ui {
class AddCloudTask;
}

class AddCloudTask : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddCloudTask(ThunderCore *tc, QWidget *parent = 0);
    ~AddCloudTask();

    enum RequestType
    {
        AddSingleTaskPre,
        AddSingleTask,
        AddBatchTaskPre,
        AddBatchTask
    };

    QString getSingleTaskURL ();
    void setSingleTaskInfo (Thunder::RemoteTask remote_task);
    
private slots:
    void on_url_textChanged(const QString &arg1);

    void slotRemoteTaskChanged (ThunderCore::RemoteTaskType type);

    void on_buttonBox_accepted();

    void on_uploadBTFile_clicked();

    void on_openEditorBtn_clicked();

    void on_getClipboardBtn_clicked();

protected:
    void showEvent(QShowEvent *e);

private:
    Ui::AddCloudTask *ui;

    ThunderCore *tcore;

    Thunder::RemoteTask tmp_singleRemoteTask;

    QStandardItemModel *bt_model, *batch_model;

signals:
    void requestChanged (AddCloudTask::RequestType type);

};

#endif // ADDCLOUDTASK_H
