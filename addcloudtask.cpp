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

#include "addcloudtask.h"
#include "ui_addcloudtask.h"

#define OFFSET_SUBID 1
#define OFFSET_SIZE 2

AddCloudTask::AddCloudTask(ThunderCore *tc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddCloudTask),
    tcore (tc),
    bt_model(new QStandardItemModel)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    ui->tableViewBT->setModel(bt_model);

    bt_model->setHorizontalHeaderLabels(QStringList()
                                        << tr("Size")
                                        << tr("Name"));

    connect (tcore, SIGNAL(RemoteTaskChanged(ThunderCore::RemoteTaskType)),
             SLOT(slotRemoteTaskChanged(ThunderCore::RemoteTaskType)));
}

AddCloudTask::~AddCloudTask()
{
    delete ui;
}

void AddCloudTask::showEvent(QShowEvent *e)
{
    switch (ui->tabWidget->currentIndex())
    {
    case 0:
    {
        QClipboard *clip = QApplication::clipboard();
        ui->url->setText(clip->text());
        ui->url->setFocus();
    }
        break;
    case 1:
        break;
    }

    QDialog::showEvent(e);
}

void AddCloudTask::slotRemoteTaskChanged(ThunderCore::RemoteTaskType type)
{
    switch (type)
    {
    case ThunderCore::SingleTaskReady:
    {
        tmp_singleRemoteTask = tcore->getSingleRemoteTask();
        tmp_singleRemoteTask.url = ui->url->text();

        ui->sizeLabel->setText(tr("File size: %1").arg(tmp_singleRemoteTask.size));
        ui->name->setText(tmp_singleRemoteTask.name);
    }
        break;
    case ThunderCore::BitorrentTaskReady:
    {
        bt_model->setRowCount(0);
        const Thunder::BitorrentTask & bttask = tcore->getUploadedBTTasks();

        ui->sizeLabelBT->setText(tr("%1 (Total: %2)")
                                 .arg(bttask.ftitle)
                                 .arg(Util::toReadableSize(bttask.btsize)));

        foreach (const Thunder::BTSubTask & task, bttask.subtasks)
        {
            QList<QStandardItem*> items;
            items << new QStandardItem (task.format_size)
                  << new QStandardItem (task.name);

            for (int i = 0; i < items.size(); ++i)
                items.at(i)->setTextAlignment(Qt::AlignCenter);

            items.first()->setData(task.id, Qt::UserRole + OFFSET_SUBID);
            items.first()->setData(task.size, Qt::UserRole + OFFSET_SIZE);

            bt_model->appendRow(items);
        }

        ui->tableViewBT->selectAll();
    }
        break;
    default:
        Q_ASSERT(false);
        break;
    }
}

void AddCloudTask::on_url_textChanged(const QString &arg1)
{
    tcore->addCloudTaskPre(arg1);
}

void AddCloudTask::setSingleTaskInfo(Thunder::RemoteTask remote_task)
{
    ui->url->setText(remote_task.name);
    ui->sizeLabel->setText(tr("Task would consume %1 disk size.").arg(remote_task.size));
}

void AddCloudTask::on_buttonBox_accepted()
{
    switch (ui->tabWidget->currentIndex())
    {
    case 0:
    {
        tcore->addCloudTaskPost(tmp_singleRemoteTask);
    }
        break;
    case 1:
    {
        QList<Thunder::BTSubTask> tasks;

        foreach (const QModelIndex & index,
                 ui->tableViewBT->selectionModel()->selectedIndexes())
            if (index.column() == 0)
            {
                Thunder::BTSubTask task;
                task.id = QString::number(index.row());
                task.size = bt_model->data(index, Qt::UserRole + OFFSET_SIZE).toString();

                tasks.append(task);
            }

        tcore->commitBitorrentTask (tasks);
        break;
    }

    default:
        break;
    }
}

void AddCloudTask::on_uploadBTFile_clicked()
{
    const QString & file = QFileDialog::getOpenFileName(this,
                                                        tr("Upload a torrent file"),
                                                        QApplication::applicationDirPath(),
                                                        tr("BT File (*.torrent);;All Files (*)"));

    if (file.isEmpty())
        return;

    tcore->uploadBitorrent(file);
}
