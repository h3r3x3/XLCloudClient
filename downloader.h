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

#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QFile>
#include <QTime>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>

class Downloader : public QObject
{
    Q_OBJECT

public:
    enum TaskStatusX
    {
        Finished,
        Failed,
        Running,
        Paused
    };

    struct TaskInfoX
    {
        unsigned long long transfered , total;
        int speed , eta;
        double percentage;
    } currentTaskInfo;

    explicit Downloader(QObject *parent = 0);
    ~Downloader();

    void setCookieJar (QNetworkCookieJar *cj);
    void cancelAndRemove ();

    unsigned long long getFileSize () { return file_size; }

    bool running;
    bool requestShutdown;
    int downBufferSize;

public slots:
    void stop();
    void startDownload ( const QString & url , const QString & absolutePath);

private:
    QNetworkAccessManager *nam;
    QString absolutePath , previousDownloadStatus;
    // readBytes: begin point <--> read bytes
    // status:    end point   <--> begin point + read bytes
    QMap<unsigned long long,unsigned long long> readBytes , status;
    QMap<unsigned long long,QByteArray> downloadBuffers;
    QFile fp , sfp;

    // transfered: real data written to disk
    // last_transfered: last time finished , how many bytes written to disk
    // _non_cache_transfered: this time tranfer (doesn't include last successfully wirrtn bytes
    unsigned long long transfered , last_transfered , _non_cache_transfered;
    int time_used , finished_thread , _non_cache_time_used;
    bool interrupted /*, shouldReadBytes*/;
    QTimer speedTimer , logSaveTimer/* , readyReadTimer*/;

    unsigned long long file_size;

private slots:
    void saveLog ();

signals:
    void readyToCloseWindow ();
    void taskStatusChanged (Downloader::TaskStatusX);

//    void taskUrlRedir ( const QString & origUrl , const QString & redirectedUrl );

private slots:
    void calcSpeed ();
    void finishedSize ();
    void finishedTransfer ();
    void readyRead ();
};

#endif // DOWNLOADER_H
