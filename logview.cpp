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

#include "logview.h"
#include "ui_logview.h"

LogView::LogView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogView)
{
    ui->setupUi(this);
}

LogView::~LogView()
{
    delete ui;
}

void LogView::logReceived(const QString &body, ThunderCore::ErrorCategory cate)
{
    QString str;

    switch (cate)
    {
    case ThunderCore::Info:
        str.append("[INFO] ");
        break;
    case ThunderCore::Notice:
        str.append("[NOTICE] ");
        break;
    case ThunderCore::Warning:
        str.append("[WARNING] ");
        break;
    case ThunderCore::Critical:
        str.append("[CRITICAL] ");
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    str.append(body);
    ui->textBrowser->append(str);
}
