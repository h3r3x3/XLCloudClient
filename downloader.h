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
