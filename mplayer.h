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
#ifndef MPLAYER_H
#define MPLAYER_H

#include <QShowEvent>
#include <QKeyEvent>
#include <QFileDialog>
#include <QDebug>
#include <QDesktopServices>
#include <QStatusBar>

#include "qmpwidget.h"

class MPlayer : public QMPwidget
{
    Q_OBJECT
public:
    explicit MPlayer(QWidget *parent = 0);

    void setStatusBar (QStatusBar *bar);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    QStatusBar *m_statusBar;
    
signals:
    
public slots:

private slots:
    void standardOutputReady(const QString & l);
    
};

#endif // MPLAYER_H
