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

#include "downloaderchildwidget.h"
#include "ui_downloaderchildwidget.h"
#include "util.h"

DownloaderChildWidget::DownloaderChildWidget(QListWidgetItem *item,
                                             const QString &downloadUrl,
                                             const QString &fileName,
                                             const QString &folderName,
                                             QWidget *parent):
    QWidget(parent),
    m_fileName(fileName),
    m_folderName(folderName),
    m_url(downloadUrl),
    m_percentage (1),
    m_autoOpen (false),
    m_item(item),
    ui(new Ui::DownloaderChildWidget)
{
    ui->setupUi(this);
    setLayout (ui->horizontalLayout_2);

    ui->label->setAttribute(Qt::WA_TranslucentBackground , true);
    ui->label_2->setAttribute(Qt::WA_TranslucentBackground , true);
    ui->label_3->setAttribute(Qt::WA_TranslucentBackground , true);
    ui->openFolderLabel->setAttribute(Qt::WA_TranslucentBackground , true);
    ui->openFileLabel->setAttribute(Qt::WA_TranslucentBackground , true);
    ui->transferStatusLabel->setAttribute(Qt::WA_TranslucentBackground , true);
    ui->fileIcon->setAttribute(Qt::WA_TranslucentBackground , true);

    connect (&m_taskStatusRoutineTimer,
             SIGNAL(timeout()), SLOT(getCurrentTaskStatus()));
    connect (&m_Downloader, SIGNAL(taskStatusChanged(Downloader::TaskStatusX)) ,
             this , SLOT(taskStatusChanged(Downloader::TaskStatusX)));
    ///
    ui->label->setText(fileName);
    ui->fileIcon->setPixmap(Util::getFileAttr(m_fileName).icon.pixmap(24));

    QList<QNetworkCookie> cookies = Util::parseMozillaCookieFile(
                QDesktopServices::storageLocation(QDesktopServices::HomeLocation)
                + "/.tdcookie");
    QNetworkCookieJar *cj = new QNetworkCookieJar;

    cj->setCookiesFromUrl(cookies , QUrl (m_url));
    m_Downloader.setCookieJar(cj);

    m_Downloader.startDownload(m_url , m_folderName + QDir::separator() + m_fileName);
}

QSize DownloaderChildWidget::sizeHint()
{
    // TODO: calculate dynamic height
    QSize size (QWidget::sizeHint().width(), 54);
    return size;
}

DownloaderChildWidget::~DownloaderChildWidget()
{
    delete ui;
}

void DownloaderChildWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    if ( (int)m_percentage == 100 )
    {
        if (m_autoOpen)
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile
                                      (m_folderName + QDir::separator() + m_fileName));
        }

        on_openFileLabel_linkActivated("");
        return;
    }

    if ( m_Downloader.running )
    {
        m_Downloader.stop();
    }
    else
    {
        m_Downloader.startDownload(m_url,
                                   m_folderName + QDir::separator() + m_fileName);
    }
}

void DownloaderChildWidget::getCurrentTaskStatus()
{
    const Downloader::TaskInfoX & taskInfo = m_Downloader.currentTaskInfo;

    QTime time (0,0,0);

    ui->transferStatusLabel->setText(QString ("%1/%2  %3/s  %4")
                                     .arg(Util::toReadableSize(taskInfo.transfered))
                                     .arg(Util::toReadableSize(taskInfo.total))
                                     .arg(Util::toReadableSize(taskInfo.speed))
                                     .arg(time.addSecs(taskInfo.eta).toString()));

    m_percentage = taskInfo.percentage / 100;
    update ();
}

void DownloaderChildWidget::taskStatusChanged(Downloader::TaskStatusX ts)
{
    switch (ts)
    {
    case Downloader::Finished:
        m_taskStatusRoutineTimer.stop();
        m_percentage = 100;
        ui->transferStatusLabel->setText(QString ("%1/%1 Finished")
                                         .arg(Util::toReadableSize(m_Downloader.getFileSize())) );
        update ();
        break;
    case Downloader::Paused:
        ui->transferStatusLabel->setText(tr("0B/0B Suspended"));
        m_taskStatusRoutineTimer.stop();
        break;
    case Downloader::Failed:
        ui->transferStatusLabel->setText(tr("0B/0B Failed"));
        m_taskStatusRoutineTimer.stop();
        break;
    case Downloader::Running:
        m_taskStatusRoutineTimer.start(1000);
        break;
    default:
        Q_ASSERT(false);
    }
}

void DownloaderChildWidget::copyURLtoClipboard(const QString &txt)
{
    QClipboard *clip = QApplication::clipboard();
    clip->setText(txt);
}

void DownloaderChildWidget::keyPressEvent(QKeyEvent *e)
{
    switch (e->modifiers())
    {
    case Qt::CTRL:
        switch (e->key())
        {
        case Qt::Key_C:
            copyURLtoClipboard(m_url);
            break;
        }

        break;
    }
}

void DownloaderChildWidget::paintEvent(QPaintEvent *)
{
    int mid = width() * m_percentage;

    QPainter painter (this);
    painter.setPen(QPen(QColor(Qt::white)));
    painter.setBrush(QBrush(QColor::fromRgb(213, 231, 245)));
    painter.drawRect(0 , 0 , mid , height() );

    painter.setBrush(QBrush(QColor("#EFF8FF")));
    painter.drawRect(mid , 0 , width() - mid , height());
}

void DownloaderChildWidget::on_openFileLabel_linkActivated(const QString &link)
{
    Q_UNUSED (link);
    QDesktopServices::openUrl(
                QUrl::fromLocalFile(m_folderName + QDir::separator() + m_fileName));
}

void DownloaderChildWidget::on_openFolderLabel_linkActivated(const QString &link)
{
    Q_UNUSED (link);
    QDesktopServices::openUrl(m_folderName);
}

bool DownloaderChildWidget::question(const QString &msg)
{
    return QMessageBox::No ==
            QMessageBox::question(this, tr("Question"),
                                  msg, QMessageBox::Yes, QMessageBox::No);
}

void DownloaderChildWidget::on_trashTaskBtn_clicked()
{
    if (question(tr("Remove task ?")))
    {
        return;
    }

    if (! question(tr("Also remove files ?")) )
    {
        m_Downloader.cancelAndRemove ();
    }

    /// this is stupid , why should one iterate and do this ??
    for ( int i = 0 ; i < m_item->listWidget()->count() ; ++ i )
    {
        if ( m_item->listWidget()->item(i) == m_item )
        {
            delete m_item->listWidget()->takeItem(i);
            break;
        }
    }
}
