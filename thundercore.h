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

#ifndef THUNDERCORE_H
#define THUNDERCORE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QWebElement>
#include <QWebFrame>
#include <QWebPage>
#include <QNetworkRequest>
#include <QNetworkCookieJar>
#include <QDebug>
#include <QList>
#include <QDateTime>

#include "qjson/parser.h"
#include "CloudObject.h"
#include "util.h"

class ThunderCore : public QObject
{
    Q_OBJECT
public:
    enum LoginStatus
    {
        NoError,
        CapchaRequired,
        Failed
    };

    enum ErrorCategory
    {
        // general information
        Info,

        // neglectable error
        Notice,

        // error, but not crashing
        Warning,

        // critical error, must be closed
        Critical
    };

    /*!
     * \brief The ChangeType enum are for mainWindow only
     */
    enum ChangeType
    {
        // that getCloudTasks() is ready
        TaskChanged,

        // that getLoginStatus()
        LoginChanged,

        // that getCapchaCode()
        CapchaReady
    };

    enum RemoteTaskType
    {
        // that call getSingleRemoteTask();
        SingleTaskReady,

        // that call getUploadedBatchTasks();
        BatchTaskReady,

        // that call getUploadedBTTasks
        BitorrentTaskReady
    };

    explicit ThunderCore(QObject *parent = 0);

    void login (const QString & user, const QString & passwd);
    LoginStatus getLoginStatus ();
    QByteArray getCapchaCode ();

    QList<Thunder::Task> getCloudTasks ();
    QList<Thunder::Task> getGarbagedTasks ();
    void reloadCloudTasks ();
    void addCloudTaskPre (const QString & url);
    void addCloudTaskPost (const Thunder::RemoteTask & task);
    void removeCloudTasks (const QStringList & ids);

    void addBatchTaskPre (const QString & urls);
    void addBatchTaskPost (const QStringList & urls);

    void uploadBitorrent (const QString & file);
    void commitBitorrentTask (const QList<Thunder::BTSubTask> &tasks);
    void getContentsOfBTFolder (const Thunder::Task &bt_task);

    Thunder::RemoteTask getSingleRemoteTask ();
    Thunder::BitorrentTask getUploadedBTTasks();
    QList<Thunder::BatchTask> getUploadedBatchTasks ();

    void cleanupHistory ();

    void loginWithCapcha (const QByteArray & capcha);
    
signals:
    void error (const QString & body, ThunderCore::ErrorCategory category);

    /*!
     * \brief Something changed, refresh something in UI ..
     */
    void StatusChanged (ThunderCore::ChangeType pt);

    /*!
     * \brief Remote task changed, links ready!
     *        In use of further notification for UI
     * \param rt
     */
    void RemoteTaskChanged (ThunderCore::RemoteTaskType rt);

    void BTSubTaskReady (const Thunder::BitorrentTask & task);

private:
    QList<Thunder::Task> tc_cloudTasks, tc_garbagedTasks;
    void parseCloudPage (const QByteArray & body);
    bool tmp_cookieIsStored;

    Thunder::RemoteTask tmp_singleTask;
    Thunder::BitorrentTask tmp_btTask;
    QList<Thunder::BatchTask> tmp_batchTasks;

    /* passwd should be cleared after use! */
    QString tc_userName, tc_passwd;
    QHash<QString, QString> tc_session;
    LoginStatus tc_loginStatus;
    QByteArray tc_capcha;
    
    QNetworkAccessManager *tc_nam;
    void get (const QUrl & url);
    void post (const QUrl & url, const QByteArray & body);

private slots:
    void slotFinished (QNetworkReply *reply);

public slots:
    /*!
     * \brief Set capcha from user input
     * \param code
     */
    void setCapcha (const QString & code);
    
};

#endif // THUNDERCORE_H
