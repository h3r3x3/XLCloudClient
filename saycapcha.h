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

#ifndef SAYCAPCHA_H
#define SAYCAPCHA_H

#include <QDialog>
#include <QPixmap>
#include <QDebug>

namespace Ui {
class SayCapcha;
}

class SayCapcha : public QDialog
{
    Q_OBJECT
    
public:
    explicit SayCapcha(QWidget *parent = 0);
    ~SayCapcha();

    void setImage (const QByteArray & data);

private slots:
    void on_code_returnPressed();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

signals:
    void capchaReady (const QString & capcha);

private:
    Ui::SayCapcha *ui;
};

#endif // SAYCAPCHA_H
