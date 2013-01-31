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

#ifndef DOWNLOADERCHILDWIDGET_H
#define DOWNLOADERCHILDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QDesktopServices>
#include <QTimer>
#include <QMessageBox>
#include <QMenu>
#include <QClipboard>
#include <QDir>
#include <QListWidgetItem>
#include <QFileIconProvider>
#include <QAction>
#include <QKeyEvent>
#include <QFile>
#include <QNetworkCookie>
#include <QUrl>

#include "downloader.h"
#include "util.h"
#include "mediaplayer.h"

namespace Ui {
class DownloaderChildWidget;
}

class DownloaderChildWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DownloaderChildWidget(QListWidgetItem *item,
                                   const QString & downloadUrl,
                                   const QString & fileName,
                                   const QString & folderName,
                                   QWidget *parent = 0);
    ~DownloaderChildWidget();

    void copyURLtoClipboard (const QString & txt);

    QString m_fileName, m_folderName, m_url;
    double m_percentage;
    bool m_autoOpen;
    QListWidgetItem *m_item; // row in list widget

    QSize sizeHint();

signals:
    void ItemDeleted (int);
    
private:
    Ui::DownloaderChildWidget *ui;
    Downloader m_Downloader;
    QTimer m_taskStatusRoutineTimer;

    bool question (const QString & msg);

protected:
    void paintEvent(QPaintEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);

private slots:
    void taskStatusChanged (Downloader::TaskStatusX ts);
    void getCurrentTaskStatus ();

    void on_openFileLabel_linkActivated(const QString &link);
    void on_openFolderLabel_linkActivated(const QString &link);
    void on_trashTaskBtn_clicked();
};

#endif // DOWNLOADERCHILDWIDGET_H
