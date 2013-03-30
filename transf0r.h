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

#ifndef TRANSF0R_H
#define TRANSF0R_H

#include <QWidget>
#include <QListWidgetItem>

#include "CloudObject.h"
#include "downloaderchildwidget.h"

namespace Ui {
class Transf0r;
}

class Transf0r : public QWidget
{
    Q_OBJECT
    
public:
    explicit Transf0r(QWidget *parent = 0);
    ~Transf0r();

    void setStoragePath (const QString & path);
    void addCloudTask (const Thunder::RemoteTask & taskInfo, bool autoOpen = false);
    
private slots:
    void slotItemCanDelete (int id);

    void on_label_linkActivated(const QString &link);

private:
    Ui::Transf0r *ui;

    QString my_storagePath;
};

#endif // TRANSF0R_H
