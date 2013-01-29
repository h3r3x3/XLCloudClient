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
