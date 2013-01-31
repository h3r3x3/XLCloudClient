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

#include "browser.h"
#include "ui_browser.h"

Browser::Browser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Browser)
{
    ui->setupUi(this);
    ui->url->setWebView(ui->webView);
    ui->loading->setMovie(new QMovie (":/resources/images/loading.gif"));
}

Browser::~Browser()
{
    delete ui;
}

void Browser::on_reload_clicked()
{
    ui->webView->reload();
}

void Browser::on_fwd_clicked()
{
    ui->webView->forward();
}

void Browser::on_stop_clicked()
{
    ui->webView->stop();
}

void Browser::on_webView_linkClicked(const QUrl &arg1)
{
    qDebug() << "Clicked on: " << arg1;
    ui->webView->load(arg1);
}

void Browser::on_webView_loadStarted()
{
    ui->loading->show();
    ui->loading->movie()->start();
}

void Browser::on_webView_loadFinished(bool arg1)
{
    Q_UNUSED(arg1);
    ui->loading->hide();
    ui->loading->movie()->stop();
}
