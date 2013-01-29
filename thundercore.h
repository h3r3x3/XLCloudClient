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

        BatchTaskReady,

        // that call getUploadedBTTasks
        BitorrentTaskReady
    };

    explicit ThunderCore(QObject *parent = 0);

    QNetworkAccessManager *getNAM ();

    void login (const QString & user, const QString & passwd);
    LoginStatus getLoginStatus ();
    QByteArray getCapchaCode ();

    QList<Thunder::Task> getCloudTasks ();
    QList<Thunder::Task> getGarbagedTasks ();
    void addCloudTaskPre (const QString & url);
    void addCloudTaskPost (const Thunder::RemoteTask & task);
    void removeCloudTasks (const QStringList & ids);

    void uploadBitorrent (const QString & file);
    void commitBitorrentTask (const QList<Thunder::BTSubTask> &tasks);

    Thunder::RemoteTask getSingleRemoteTask ();
    Thunder::BitorrentTask getUploadedBTTasks();
    
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

private:
    QList<Thunder::Task> tc_cloudTasks, tc_garbagedTasks;
    void parseCloudPage (const QByteArray & body);

    Thunder::RemoteTask tmp_singleTask;
    Thunder::BitorrentTask tmp_btTask;

    /* passwd should be cleared after use! */
    QString tc_userName, tc_passwd;
    QHash<QString, QString> tc_session;
    LoginStatus tc_loginStatus;
    QByteArray tc_capcha;

    void loginWithCapcha (const QByteArray & capcha);
    
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
