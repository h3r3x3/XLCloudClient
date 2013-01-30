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

#ifndef CLOUDOBJECT_H
#define CLOUDOBJECT_H

#include <QString>
#include <QUrl>
#include <QIcon>

namespace Thunder
{
    enum TaskType
    {
        Single,
        BT,
        Garbage
    };

    struct RemoteTask
    {
        QString name;
        QString size;

        QString url;
    };


    struct BTSubTask
    {
        QString id;
        QString name;
        QString size;
        QString format_size;
    };

    struct BitorrentTask
    {
        QString ftitle;
        QString infoid;
        unsigned long long btsize;
        QList<BTSubTask> subtasks;
    };

    struct Task
    {
        /*!
         * \brief Task ID
         */
        QString id;

        /*!
         * \brief status 2=Done 1=No
         */
        int status;

        QString cid;

        /*!
         * \brief Task size in bytes
         */
        unsigned long long size;

        /*!
         * \brief Task type, i.e single file or BT
         */
        TaskType type;

        /*!
         * \brief Display task name
         */
        QString name;

        /*!
         * \brief Download link
         */
        QString link, bt_url;

        /*!
         * \brief Source address
         */
        QString source;

        bool finished()
        {
            return status == 2;
        }

        bool isEmpty()
        {
            return name.isEmpty();
        }
    };

    struct File
    {
        QString extension;
        QIcon   icon;
    };

}

#endif // CLOUDOBJECT_H
