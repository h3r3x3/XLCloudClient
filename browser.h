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

#ifndef BROWSER_H
#define BROWSER_H

#include <QWidget>
#include <QUrl>
#include <QDebug>

namespace Ui {
class Browser;
}

class Browser : public QWidget
{
    Q_OBJECT
    
public:
    explicit Browser(QWidget *parent = 0);
    ~Browser();
    
private slots:
    void on_url_returnPressed();
    void on_webView_urlChanged(const QUrl &arg1);
    void on_reload_clicked();

    void on_fwd_clicked();
    void on_stop_clicked();

    void on_webView_linkClicked(const QUrl &arg1);

    void on_search_returnPressed();

private:
    Ui::Browser *ui;
};

#endif // BROWSER_H
