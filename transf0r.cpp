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

#include "transf0r.h"
#include "ui_transf0r.h"

Transf0r::Transf0r(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Transf0r),
    my_storagePath("/dev/shm/")
{
    ui->setupUi(this);
}

Transf0r::~Transf0r()
{
    delete ui;
}

void Transf0r::setStoragePath(const QString &path)
{
    if (! path.isEmpty())
        my_storagePath = path;
}

void Transf0r::addCloudTask(const Thunder::RemoteTask &taskInfo, bool autoOpen)
{
    QListWidgetItem *item = new QListWidgetItem;
    ui->listWidget->addItem(item);

    DownloaderChildWidget *cw = new DownloaderChildWidget (item,
                                                           taskInfo.url,
                                                           taskInfo.name,
                                                           my_storagePath, this);
    cw->m_autoOpen = autoOpen;

    connect(cw, SIGNAL(ItemDeleted(int)), SLOT(slotItemCanDelete(int)));

    item->setSizeHint(cw->sizeHint());
    ui->listWidget->setItemWidget(item, cw);
}

void Transf0r::slotItemCanDelete(int id)
{
    delete ui->listWidget->takeItem(id - 1);
}
