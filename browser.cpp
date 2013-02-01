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

    QMenu *searchMenu = new QMenu (this);
    QAction *action = new QAction (QIcon(":/resources/images/yyets.png"),
                                   tr("YYeTS"), this);
    connect (action, SIGNAL(triggered()), SLOT(set_yyets()));
    searchMenu->addAction(action);

    action = new QAction (QIcon(":/resources/images/amule_mugen.png"),
                          tr("VeryCD"), this);
    connect (action, SIGNAL(triggered()), SLOT(set_verycd()));
    searchMenu->addAction(action);

    ui->search->setMenu(searchMenu);

    ///
    connect (ui->search, SIGNAL(returnPressed()),
             SLOT(returnPressed()));
}

Browser::~Browser()
{
    delete ui;
}

void Browser::returnPressed ()
{
    switch (m_engine)
    {
    case VeryCD:
        ui->webView->load(QString("http://www.verycd.com/search/entries/%1")
                          .arg(ui->search->text()));
        break;
    case YYeTS:
        ui->webView->load(QString("http://www.yyets.com/php/search/index?keyword=%1")
                          .arg(ui->search->text()));
        break;
    }
}

void Browser::set_verycd()
{
    m_engine = VeryCD;
    ui->search->setInactiveText("VeryCD");
}

void Browser::set_yyets()
{
    m_engine = YYeTS;
    ui->search->setInactiveText("YYeTS");
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
