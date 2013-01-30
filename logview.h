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

#ifndef LOGVIEW_H
#define LOGVIEW_H

#include <QWidget>
#include "thundercore.h"

namespace Ui {
class LogView;
}

class LogView : public QWidget
{
    Q_OBJECT
    
public:
    explicit LogView(QWidget *parent = 0);
    ~LogView();

private slots:
    void logReceived (const QString & body, ThunderCore::ErrorCategory cate);
    
private:
    Ui::LogView *ui;
};

#endif // LOGVIEW_H
