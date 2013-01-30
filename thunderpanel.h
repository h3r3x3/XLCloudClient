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

#ifndef THUNDERPANEL_H
#define THUNDERPANEL_H

#include <QWidget>
#include <QStandardItemModel>
#include <QMenu>
#include <QAction>
#include <QCursor>
#include <QApplication>
#include <QClipboard>
#include <QStandardItem>
#include <QDebug>

#include "CloudObject.h"
#include "util.h"

namespace Ui {
class ThunderPanel;
}

class ThunderPanel : public QWidget
{
    Q_OBJECT
    
public:
    enum RequestType
    {
        Download,
        Preview
    };

    enum IndirectRequestType
    {
        RemoveTasks,
        AddTask
    };

    explicit ThunderPanel(QWidget *parent = 0);
    ~ThunderPanel();

    void setCloudTasks (const QList<Thunder::Task> & tasks);
    QStringList getSelectedTaskIDs ();

    Thunder::RemoteTask getFirstSelectedTask ();
    
signals:
    void doThisLink (const Thunder::RemoteTask & task, ThunderPanel::RequestType type);
    void doIndirectRequest (ThunderPanel::IndirectRequestType type);

private:
    Ui::ThunderPanel *ui;

    QStandardItemModel *my_model;
    QString getUserDataByOffset (unsigned long long offset, int row = -1);

    QMenu *my_contextMenu;

private slots:
    void slotShowContextMenu (const QPoint & pt);

    void slotPreviewThisTask ();
    void slotDownloadThisTask ();

    void slotRemoveTheseTasks ();

    void slotCopyDownloadAddress ();
    void slotCopySourceAddress ();
};

#endif // THUNDERPANEL_H
