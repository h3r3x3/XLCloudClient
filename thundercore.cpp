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

#include "thundercore.h"

ThunderCore::ThunderCore(QObject *parent) :
    QObject(parent),
    tc_nam (new QNetworkAccessManager (this))
{
    connect (tc_nam, SIGNAL(finished(QNetworkReply*)),
             SLOT(slotFinished(QNetworkReply*)));
}

QList<Thunder::Task> ThunderCore::getCloudTasks()
{
    return tc_cloudTasks;
}

QList<Thunder::Task> ThunderCore::getGarbagedTasks()
{
    return tc_garbagedTasks;
}

Thunder::RemoteTask ThunderCore::getSingleRemoteTask()
{
    return tmp_singleTask;
}

void ThunderCore::cleanupHistory()
{
    get ("http://dynamic.cloud.vip.xunlei.com/interface/history_clear"
         "?tcache=1328430359476&flag=6&uid=" + tc_session.value("userid"));
}

void ThunderCore::commitBitorrentTask(const QList<Thunder::BTSubTask> &tasks)
{
    QString ids, sizes;
    foreach (const Thunder::BTSubTask & task, tasks)
    {
        ids.append(task.id).append("_");
        sizes.append(task.size).append("_");
    }

    post (QUrl("http://dynamic.cloud.vip.xunlei.com/"
               "interface/bt_task_commit?callback=a&t=" +
               QDateTime::currentDateTime().toString().toAscii().toPercentEncoding()),
          "goldbean=0&class_id=0&o_taskid=0&o_page=task&silverbean=0"
          "&findex=" + ids.toAscii() +
          "&uid=" + tc_session.value("userid").toAscii() +
          "&btname=" + tmp_btTask.ftitle.toUtf8().toPercentEncoding() +
          "&tsize=" + QByteArray::number(tmp_btTask.btsize) +
          "&size=" + sizes.toAscii() +
          "&cid=" + tmp_btTask.infoid.toAscii());
}

void ThunderCore::addCloudTaskPost(const Thunder::RemoteTask &task)
{
    get ("http://dynamic.cloud.vip.xunlei.com/interface/"
         "task_commit?callback=ret_task&cid=&gcid=&goldbean=0&"
         "silverbean=0&type=2&o_page=task&o_taskid=0&ref_url=&size=" +
         task.size + "&uid=" + tc_session["userid"] +
            "&t=" + task.name + "&url=" + task.url);
}

void ThunderCore::loginWithCapcha(const QByteArray &capcha)
{
    error (tr("Login .. cookie: %1").arg(QString::fromAscii(capcha)), Info);

    post (QUrl("http://login.xunlei.com/sec2login/"),
          "login_hour=720&login_enable=0&u=" + tc_userName.toAscii() +
          "&verifycode=" + capcha +
          "&p=" + Util::getEncryptedPassword(
              tc_passwd, QString::fromAscii(capcha), true).toAscii());
}

void ThunderCore::getContentsOfBTFolder(const Thunder::Task &bt_task)
{
    get ("http://dynamic.cloud.vip.xunlei.com/interface/fill_bt_list"
         "?callback=a&g_net=1&p=1&noCacheIE=1328405858893&infoid=" + bt_task.cid +
         "&tid=" + bt_task.id +
         "&uid=" + tc_session.value("userid"));
}

void ThunderCore::setCapcha(const QString &code)
{
    loginWithCapcha(code.toAscii());
}

void ThunderCore::addCloudTaskPre(const QString &url)
{
    get ("http://dynamic.cloud.vip.xunlei.com/interface/"
         "task_check?callback=queryCid"
         "&random=13271369889801529719.0135479392&tcache=1327136998160&url=" + url);
}

void ThunderCore::slotFinished(QNetworkReply *reply)
{
    const QUrl & url = reply->url();
    const QString & urlStr = url.toString();
    const QByteArray & data = reply->readAll();
    int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (httpStatus < 200 || httpStatus > 400)
    {
        error (tr("Error reading %1, got %2")
               .arg(urlStr)
               .arg(httpStatus), Notice);
        return;
    }
    /////

    if (urlStr.startsWith("http://login.xunlei.com/check"))
    {
        QByteArray capcha;
        foreach (const QNetworkCookie & cookie, tc_nam->cookieJar()->cookiesForUrl(url))
        {
            if (cookie.name() == "check_result") { capcha = cookie.value(); break; }
        }

        /* Get rid of heading 0: */
        capcha = capcha.remove(0, 2);

        if (capcha.isEmpty())
        {
            error (tr("Capcha required, retrieving .."), Notice);
            tc_loginStatus = Failed; emit StatusChanged (LoginChanged);

            /* TODO: */
            get (QString("http://verify.xunlei.com/image?cachetime=1359365355018"));

            return;
        }

        loginWithCapcha(capcha);

        return;
    }

    if (urlStr.startsWith("http://verify.xunlei.com/image"))
    {
        tc_capcha = data;
        error (tr("Capcha received, size %1").arg(tc_capcha.size()), Notice);

        emit StatusChanged(CapchaReady);

        return;
    }

    if (urlStr.startsWith("http://login.xunlei.com/sec2login/"))
    {
        foreach (const QNetworkCookie & cookie, tc_nam->cookieJar()->cookiesForUrl(url))
        {
            if (cookie.name() == "usernick")
            {
                error (tr("User nick: %1").arg(QString::fromAscii(cookie.value())), Info);
            }

            tc_session.insert(cookie.name(), cookie.value());
        }

        if (! tc_session.contains("jumpkey"))
        {
            error (tr("Logon failure"), Warning);
            tc_loginStatus = Failed; emit StatusChanged (LoginChanged);

            return;
        }

        get (QUrl("http://dynamic.cloud.vip.xunlei.com/login?"
                  "cachetime=1327129660280&cachetime=1327129660555&from=0"));

        return;
    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/login"))
    {
        get (QUrl("http://dynamic.cloud.vip.xunlei.com/user_task?st=0&userid=" +
                  tc_session.value("userid")));

        return;
    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/user_task"))
    {
        /*
        * It's best that we handle the page with QtScript!
        */

        error (tr("Retrieved user page, analysing .."), Info);
        parseCloudPage(data);

        return;
    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/interface/task_delete"))
    {
        /*!
         * \brief Reload user page!
         */
        get (QUrl("http://dynamic.cloud.vip.xunlei.com/user_task?st=0&userid=" +
                  tc_session.value("userid")));

        error (tr("Task removed, reloading page .."), Info);
        return;
    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/interface/task_check"))
    {
        QStringList fields = Util::parseFunctionFields(data);

        if (fields.size() != 10)
        {
            error (tr("Protocol changed or parser failure, submit this line: %1")
                   .arg(QString::fromAscii(data)), Warning);
            return;
        }

        tmp_singleTask.name = fields.at(4);
        tmp_singleTask.size = Util::toReadableSize(fields.at(2).toULongLong());

        emit RemoteTaskChanged(SingleTaskReady);

        return;
    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/interface/task_commit"))
    {
        error(tr("Task added, reloading page .."), Notice);

        get (QUrl("http://dynamic.cloud.vip.xunlei.com/user_task?st=0&userid=" +
                  tc_session.value("userid")));

        return;
    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/"
                          "interface/torrent_upload"))
    {
        // todo: bt task editing?
        QByteArray json = data;

        // remove btResult =
        json.remove (0, 51);

        // chop </script>
        json.chop (10);

        QJson::Parser parser;
        bool ok = false;
        QVariant result = parser.parse(json, &ok);

        if (! ok)
        {
            error (tr("JSON parse failure, protocol changed or invalid data."),
                   Warning);
            qDebug() << json;

            return;
        }

        tmp_btTask.ftitle = result.toMap().value("ftitle").toString();
        tmp_btTask.infoid = result.toMap().value("infoid").toString();
        tmp_btTask.btsize = result.toMap().value("btsize").toULongLong();

        foreach (const QVariant & item, result.toMap().value("filelist").toList())
        {
            QVariantMap map = item.toMap();

            Thunder::BTSubTask task;
            task.name = map.value("subtitle").toString();
            task.format_size = map.value("subformatsize").toString();
            task.size = map.value("subsize").toString();
            task.id = map.value("id").toString();

            tmp_btTask.subtasks.append(task);
        }

        emit RemoteTaskChanged(BitorrentTaskReady);

        return;

    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/"
                          "interface/bt_task_commit"))
    {
        error(tr("Bitorrent commited, reloading tasks .."), Notice);

        get (QUrl("http://dynamic.cloud.vip.xunlei.com/user_task?st=0&userid=" +
                  tc_session.value("userid")));

        return;
    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/interface/history_clear"))
    {
        error(tr("History emptied"), Notice);

        // ERROR checking?

        return;
    }

    if (urlStr.startsWith("http://dynamic.cloud.vip.xunlei.com/interface/fill_bt_list"))
    {
        error(tr("BT task page retrieved, parsing .."), Notice);

        QByteArray json = data;

        // remove a(
        json.remove (0, 2);

        // chop )
        json.chop (1);

        QJson::Parser parser;
        bool ok = false;
        QVariant result = parser.parse(json, &ok);

        if (! ok)
        {
            error (tr("BT task page: JSON parse failure, "
                      "protocol changed or invalid data."),
                   Warning);
            qDebug() << json;

            return;
        }

        result = result.toMap().value("Result");

        Thunder::BitorrentTask bt_task;
        bt_task.taskid = result.toMap().value("Tid").toString();

        foreach (const QVariant & record, result.toMap().value("Record").toList())
        {
            const QVariantMap & map = record.toMap();
            Thunder::BTSubTask subtask;

            subtask.id = map.value("id").toString();
            subtask.size = map.value("size").toString();
            subtask.link = map.value("downurl").toString();
            subtask.name = map.value("dirtitle").toString();

            bt_task.subtasks.append(subtask);
        }

        emit BTSubTaskReady (bt_task);
        return;
    }

    qDebug() << "Unhandled reply:" << "\n----";
    qDebug() << "URL: " << urlStr;
    qDebug() << "Cookie: ";
    foreach (const QNetworkCookie & cookie, tc_nam->cookieJar()->cookiesForUrl(url))
    {
        qDebug() << cookie.name() << cookie.value();
    }
    qDebug() << "Data: " << data;
}

QByteArray ThunderCore::getCapchaCode()
{
    return tc_capcha;
}

Thunder::BitorrentTask ThunderCore::getUploadedBTTasks()
{
    return tmp_btTask;
}

void ThunderCore::parseCloudPage(const QByteArray &body)
{
    QWebPage page;
    page.settings()->setAttribute(QWebSettings::JavaEnabled, false);
    page.settings()->setAttribute(QWebSettings::JavascriptEnabled, false);
    page.settings()->setAttribute(QWebSettings::AutoLoadImages, false);

    page.mainFrame()->setHtml(QString::fromUtf8(body));
    //    tc_session.clear();

    /// FIND gdriveid
    foreach (const QWebElement & input, page.mainFrame()->findAllElements("input"))
        if (input.attribute("id") == "cok")
        {
            tc_session.insert("gdriveid", input.attribute("value"));
            break;
        }

    if (! tc_session.contains("gdriveid"))
    {
        error (tr("Broken page! No gdrive id found, "
                  "you will not be able to initiate any file transfer! "), Warning);
        return;
    }

    tc_cloudTasks.clear();

    /// FIND TASKS
    foreach (const QWebElement & div, page.mainFrame()->findAllElements("div"))
    {
        if (! div.attributeNames().contains("taskid")) continue;

        Thunder::Task task;
        task.id = div.attribute("taskid");
        task.type = Thunder::Single;

        foreach (const QWebElement & input, div.findAll("input"))
        {
            const QString & id = input.attribute("id");
            if (id.startsWith("f_url"))       task.source = input.attribute("value");
            if (id.startsWith("dcid"))        task.cid    = input.attribute("value");
            if (id.startsWith("durl"))        task.name   = input.attribute("value");
            if (id.startsWith("dl_url"))      task.link   = input.attribute("value");
            if (id.startsWith("bt_down_url")) task.bt_url = input.attribute("value");
            if (id.startsWith("ysfilesize"))  task.size   = input.attribute("value").toULongLong();
            if (id.startsWith("d_status"))    task.status = input.attribute("value").toInt();
        }

        if (! task.isEmpty())
        {
            if (task.bt_url.startsWith("bt://"))
                task.type = Thunder::BT;
            tc_cloudTasks.push_back(task);
        }
    }

    error (tr("%1 task(s) loaded.").arg(tc_cloudTasks.size()), Notice);
    emit StatusChanged(TaskChanged);
}

void ThunderCore::removeCloudTasks(const QStringList &ids)
{
    post (QUrl("http://dynamic.cloud.vip.xunlei.com/interface/task_delete?type=2&callback=a"),
          "databases=0,&old_databaselist=&old_idlist=&taskids=" + ids.join(",").toAscii());
}

void ThunderCore::get(const QUrl &url)
{
    tc_nam->get(QNetworkRequest(url));
}

void ThunderCore::uploadBitorrent(const QString &file)
{
    tmp_btTask.subtasks.clear();

    const QByteArray & torrent = Util::readWholeFile(file);
    const QByteArray & boundary = Util::getRandomString(10).toAscii();

    QNetworkRequest request (QString("http://dynamic.cloud.vip.xunlei.com/"
                                     "interface/torrent_upload"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "multipart/form-data; boundary=----" + boundary);
    tc_nam->post(request, "------" + boundary + "\n"
                 "Content-Disposition: form-data; "
                 "name=\"filepath\"; filename=sample.torrent\n"
                 "Content-Type: application/x-bitorrent\n\n" + torrent +
                 "Content-Disposition: form-data; name=\"random\"\n\n" +
                 "13284335922471757912.3826739355\n"
                 "------" + boundary + "\n");
}

void ThunderCore::post(const QUrl &url, const QByteArray &body)
{
    QNetworkRequest request (url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    tc_nam->post(request, body);
}

void ThunderCore::login(const QString &user, const QString &passwd)
{
    tc_userName = user;
    tc_passwd   = passwd;

    error ("Getting capcha code ..", Info);
    this->get(QString("http://login.xunlei.com/check?u=%1&cachetime=%2")
              .arg(user)
              .arg(QString::number(QDateTime::currentMSecsSinceEpoch())));
}
