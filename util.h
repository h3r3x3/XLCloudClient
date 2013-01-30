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

#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include <QCryptographicHash>
#include <QDebug>
#include <QNetworkCookie>
#include <QList>
#include <QFile>
#include <QDesktopServices>
#include <QDateTime>

#include <cstdio>
#include "CloudObject.h"

static QHash<QString, QString> static_iconMapping;

class Util : public QObject
{
    Q_OBJECT
public:
    explicit Util(QObject *parent = 0);

    static void init ();

    /*!
     * \brief Encrypt password
     * \param plain text password
     * \param verify code
     * \param We store encrypted password only.
     * \return
     */
    static QString getEncryptedPassword (const QString &pass,
                                         const QString &code,
                                         bool encrypted = false);

    static QString getMD5Hex (const QString & pass);

    /*!
     * \brief Convert number to human readable size
     * \param size
     * \return
     */
    static QString toReadableSize (const unsigned long long &size);

    /*!
     * \brief Return file attribute and icon
     * \param fileName
     * \return
     */
    static Thunder::File getFileAttr (const QString & fileName);

    /*!
     * \brief Parse function fields, i.e queryCid('', '', 0, 'Movie')
     * \param data
     * \return
     */
    static QStringList parseFunctionFields (const QByteArray & d);

    /*!
     * \brief Return a set of cookie from a mozilla styled cookie file
     * \param file
     * \return
     */
    static QList<QNetworkCookie> parseMozillaCookieFile (const QString & file);

    /*!
     * \brief Read whole file into a QByteArray()
     * \param file
     * \return
     */
    static QByteArray readWholeFile (const QString & file);

    /*!
     * \brief Generate random alpha number string
     * \param len
     * \return
     */
    static QString getRandomString (const int & len = 15);

    /*!
     * \brief Get absolute path of user home directory
     * \return
     */
    static QString getHomeLocation ();
    
signals:
    
public slots:
    
};

#endif // UTIL_H
