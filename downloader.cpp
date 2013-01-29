#include "downloader.h"

#define LOG_SUFFIX ".td"

static const int thread_count = 5;
static const QRegExp ContentRangeRegEx ("bytes ([0-9]+)-([0-9]+)/([0-9]+)");

Downloader::Downloader(QObject *parent):
    QObject (parent) ,
    running (false),
    requestShutdown (false),
    downBufferSize ( 3*1024*1024 )
{
    nam = new QNetworkAccessManager (this);
    connect ( &speedTimer , SIGNAL(timeout()) , SLOT(calcSpeed()) );
    connect ( &logSaveTimer , SIGNAL(timeout()) , SLOT(saveLog()) );
}

Downloader::~Downloader()
{

}

void Downloader::setCookieJar(QNetworkCookieJar *cj)
{
    nam->setCookieJar (cj);
}

void Downloader::calcSpeed()
{
    // real time used
    time_used += speedTimer.interval() / 1000;
    // this transfer
    _non_cache_time_used += speedTimer.interval() / 1000;
    if ( time_used != 0 )
    {
        unsigned long long transfered_total = _non_cache_transfered + last_transfered;

        int speed = _non_cache_transfered / _non_cache_time_used;
        double percentage = (double)transfered_total * 100 / (double)file_size;
        int eta = (double) ( file_size - _non_cache_transfered ) / speed;

        currentTaskInfo.eta = eta;
        currentTaskInfo.transfered = transfered_total;
        currentTaskInfo.speed = speed;
        currentTaskInfo.percentage = percentage;

//        qDebug() << "Per: " << percentage << " Eta: " << eta << "  Tx: " << _non_cache_transfered + last_transfered << "  Tot: " << file_size;
    }
}

void Downloader::cancelAndRemove ()
{
    stop();
    QFile fpX;
    fpX.setFileName(absolutePath);
    fpX.remove();

    fpX.setFileName(absolutePath + LOG_SUFFIX);
    fpX.remove();
}

void Downloader::finishedSize ()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if ( reply->error() )
    {
        emit taskStatusChanged(Failed);
        running = false;
        return;
    }

    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if ( ! redirectionTarget.isNull() )
    {
//        emit taskUrlRedir( reply->url().toString() , redirectionTarget.toUrl().toString() );
        qDebug() << "Redir: " << redirectionTarget.toUrl().toString();

        // retrieve file size firstly
        QNetworkRequest request ( redirectionTarget.toUrl() );
        request.setRawHeader("Range" , "bytes=1-5");

        QNetworkReply *reply = nam->get( request );
        connect (reply , SIGNAL(finished()) , SLOT(finishedSize()));
        return;
    }

    if ( ContentRangeRegEx.indexIn( reply->rawHeader("Content-Range") ) != -1 )
    {
        bool ok = false;
        file_size = ContentRangeRegEx.cap(3).toULongLong(&ok);

        memset (&currentTaskInfo , 0 , sizeof (currentTaskInfo));
        currentTaskInfo.total = file_size;

        if ( ! ok || file_size == 0 )
        {
            qDebug() << "Target doesn't support HTTP Range";
            running = false;
            return;
        }

        if ( fp.isOpen() )
            fp.close();

        // log file
        if ( sfp.isOpen() )
            sfp.close();

        sfp.setFileName(absolutePath + LOG_SUFFIX);
        fp.setFileName(absolutePath);

        // reset parameters
        time_used = 0;
        transfered = 0;
        finished_thread = 0;
        _non_cache_time_used = 0;
        interrupted = false;
        status.clear();
        downloadBuffers.clear();

        if ( (unsigned long long) fp.size() == file_size && ! sfp.exists() )
        {
            emit taskStatusChanged(Finished);

            running = false;
            return;
        }

        if ( sfp.exists() )
        {
            if ( sfp.open(QIODevice::ReadOnly | QIODevice::Text) )
            {
                QString line = sfp.readLine().trimmed();
                transfered = line.toULongLong();

                line = sfp.readLine().trimmed();
                time_used = line.toULongLong();

                while ( ! sfp.atEnd() )
                {
                    line = sfp.readLine();
                    int idx = line.indexOf(":");
                    if ( idx != -1 )
                        status.insert( line.left(idx).toULongLong() , line.right( line.length() - idx - 1 ).toULongLong() );
                }

                if ( status.size() == 0 )
                {
                    transfered = 0;
                }
                else
                    finished_thread = thread_count - status.size();

                //                qDebug() << "Start from:  " << transfered << "  Segleft: " << status.size();

                sfp.close();
            }
            else
            {
                qDebug() << "Error opening log file: " << sfp.errorString();
                sfp.remove();
            }
        }
        else
            fp.remove();

        if ( ! fp.open(QIODevice::ReadWrite) )
        {
            qDebug() << "Unable to write file: " << fp.errorString();
            emit taskStatusChanged(Failed);
            return;
        }

        running = true;
        _non_cache_transfered = 0;
        last_transfered = transfered;
        emit taskStatusChanged(Running);

        // start speed timer
        speedTimer.start(1000);
        logSaveTimer.start(1000);
//        readyReadTimer.start(2000);

        if ( status.isEmpty() )
        {
            unsigned long long begin = 0 , end , delta = file_size / thread_count;
            for ( int i = 0 ; i < thread_count ; ++ i )
            {
                if ( i > 0 )
                    begin = i * delta;

                if ( i == thread_count - 1 )
                    end = file_size - 1;
                else
                    end = begin + delta - 1;

//                qDebug() << "Assign: " << begin << end;

                readBytes.insert(begin , 0);

                QNetworkRequest request ( reply->url() );
                request.setRawHeader("Range" , QString ("bytes=%1-%2").arg(begin).arg(end).toAscii() );

                QNetworkReply *reply = nam->get( request );
                connect (reply , SIGNAL(readyRead()) , SLOT(readyRead()));
                connect (reply , SIGNAL(finished()) , SLOT(finishedTransfer()));
            }
        }
        else
        {

            QMap<unsigned long long,unsigned long long>::const_iterator it = status.begin();
            while ( it != status.end() )
            {

                unsigned long long begin = it.value() ,
                        end = it.key();

//                qDebug() << "ReAssign: " << begin << end;

                readBytes.insert(begin , 0);

                QNetworkRequest request ( reply->url() );
                request.setRawHeader("Range" , QString ("bytes=%1-%2").arg(begin).arg(end).toAscii() );
//                request.setRawHeader("Cookie" , _cookie.toAscii());

                QNetworkReply *reply = nam->get( request );
                connect (reply , SIGNAL(readyRead()) , SLOT(readyRead()));
                connect (reply , SIGNAL(finished()) , SLOT(finishedTransfer()));

                ++ it;

            }

        }
    }
    else
    {
        qDebug() << "Target doesn't support HTTP Range command.";
        emit taskStatusChanged(Failed);
    }
}

void Downloader::stop()
{
    interrupted = true;
}

void Downloader::saveLog()
{
    QByteArray downloadStatus;
    downloadStatus.append( QString::number (transfered).toAscii()  );
    downloadStatus.append( "\n" + QString::number (time_used).toAscii()  );

    QMap<unsigned long long,unsigned long long>::const_iterator it = status.begin();
    while ( it != status.end() )
    {
        downloadStatus.append("\n")
                .append( QString::number (it.key()).toAscii())
                .append(":")
                .append( QString::number (it.value()).toAscii() );
        ++ it;
    }

    if ( downloadStatus == previousDownloadStatus )
        return;

    previousDownloadStatus = downloadStatus;

    if ( ! sfp.open(QIODevice::WriteOnly) )
    {
        qDebug() << "Unable to write to log file: " << sfp.errorString();
        return;
    }

    if ( ! sfp.write(downloadStatus) )
    {
        qDebug() << "Incomplete log file: " << sfp.errorString();
    }

    sfp.close();
}

void Downloader::finishedTransfer()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if ( ContentRangeRegEx.indexIn( reply->rawHeader("Content-Range") ) != -1 )
    {

        if ( ! reply->error() )
        {
            // write un-finished buffer
            unsigned long long begin = ContentRangeRegEx.cap(1).toULongLong();
            unsigned long long end = ContentRangeRegEx.cap(2).toULongLong();

            if ( ! fp.seek( readBytes.value(begin) + begin ) )
            {
                qDebug() << "Seek error , data will be lost: " << fp.errorString();
                return;
            }

            fp.write(downloadBuffers.value(begin));

            transfered += downloadBuffers.value(begin).length();

            readBytes.insert( begin , readBytes.value(begin) + downloadBuffers.value(begin).length() );
            status.insert( end ,  begin + readBytes.value( begin ) );

            downloadBuffers.remove(begin);
        }

    }

    if ( ++ finished_thread == thread_count )
    {
        speedTimer.stop();
        logSaveTimer.stop();
        //        qDebug() << "Trans: " << transfered;
        //        qDebug() << "File Size: " << file_size;

        fp.close();

        running = false;

//        qDebug() << " " << time_used << " seconds";

        if ( transfered >= file_size - 1 )
        {
            sfp.remove();
            emit taskStatusChanged(Finished);
        }
        else
        {
            saveLog();

            // when mainwindow is closed
            if ( requestShutdown )
            {
                emit readyToCloseWindow();
            }
            // failure or suspended by user
            else
                emit taskStatusChanged(Paused);
        }

    }

}

void Downloader::startDownload(const QString &url , const QString & absolutePath)
{
    Q_ASSERT (running == false);

    running = true;

    //TODO: won't work for windows
    this->absolutePath = absolutePath;
    this->absolutePath.replace("\\" , "_");

    // retrieve file size firstly
    QNetworkRequest request (url);
    request.setRawHeader("Range" , "bytes=1-5");

    QNetworkReply *reply = nam->get( request );
    connect (reply , SIGNAL(finished()) , SLOT(finishedSize()));
}

void Downloader::readyRead()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if ( reply->error() )
    {
        qDebug() << "Transfer Error: " << reply->errorString();
    }

    if ( ContentRangeRegEx.indexIn( reply->rawHeader("Content-Range") ) != -1 )
    {
        unsigned long long begin = ContentRangeRegEx.cap(1).toULongLong();
        unsigned long long end = ContentRangeRegEx.cap(2).toULongLong();

        const QByteArray & data = reply->readAll();
        downloadBuffers[begin].append(data);
//        downloadBuffers.insert(begin , downloadBuffers.value(begin) + data);

//        qDebug() << begin << " Got: " << data.length() << " bytes";

        _non_cache_transfered += data.length();

        if ( interrupted || downloadBuffers.value(begin).length() > downBufferSize )
        {

            if ( ! fp.seek( readBytes.value(begin) + begin ) )
            {
                qDebug() << "Seek error , data will be lost: " << fp.errorString();
                return;
            }

            fp.write(downloadBuffers.value(begin));

            transfered += downloadBuffers.value(begin).length();

            readBytes.insert( begin , readBytes.value(begin) + downloadBuffers.value(begin).length() );
            status.insert( end ,  begin + readBytes.value( begin ) );

            downloadBuffers[begin].clear();
            downloadBuffers[begin].reserve(1.5 * downBufferSize);
        }
    }

    if ( interrupted )
    {
        reply->abort();
    }
}
