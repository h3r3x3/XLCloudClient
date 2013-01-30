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

/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Copyright: 2012 Labo A.L
 * Authors:
 *   Aaron Lewis <the.warl0ck.1989@gmail.com>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */
#include "mplayer.h"

MPlayer::MPlayer(QWidget *parent) :
    QMPwidget(parent)
{
    QMPwidget::start(QStringList()
                     << "-cache" << "2000"
                     << "-cookies"
                     << "-cookies-file"
                     << QDesktopServices::storageLocation
                     (QDesktopServices::HomeLocation) + "/.tdcookie");
}

void MPlayer::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_O:
        QString url = QFileDialog::getOpenFileName(this, tr("Play media file..."));
        if (!url.isEmpty()) {
            QMPwidget::load(url);
        }

        break;
    }
}

void MPlayer::standardOutputReady(const QString &l)
{
    m_statusBar->showMessage(l);
}

void MPlayer::setStatusBar (QStatusBar *bar)
{
    m_statusBar = bar;
    if ( m_statusBar )
    {
        m_statusBar->setStyleSheet("text-align: right; background: black; color: white");
        connect (this, SIGNAL(readStandardOutput(QString)), this, SLOT(standardOutputReady(QString)));
    }
    else
    {
        disconnect (this, SIGNAL(readStandardOutput(QString)), this, SLOT(standardOutputReady(QString)));
    }
}
