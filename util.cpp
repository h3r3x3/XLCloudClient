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

#include "util.h"

Util::Util(QObject *parent) :
    QObject(parent)
{
}

QString Util::getHomeLocation()
{
    return QDesktopServices::storageLocation(QDesktopServices::HomeLocation);
}

void Util::init()
{
    // not good design. @TODO
    static_autoOpenMapping.insert("mkv", Thunder::Video);
    static_autoOpenMapping.insert("avi", Thunder::Video);
    static_autoOpenMapping.insert("rmvb", Thunder::Video);
    static_autoOpenMapping.insert("pdf", Thunder::Document);
    static_autoOpenMapping.insert("chm", Thunder::Document);
    static_autoOpenMapping.insert("rar", Thunder::Document);

    ///
    static_iconMapping.insert("avi", ":/resources/images/movie.png");
    static_iconMapping.insert("mp4", ":/resources/images/movie.png");
    static_iconMapping.insert("mkv", ":/resources/images/movie.png");
    static_iconMapping.insert("rm", ":/resources/images/movie.png");
    static_iconMapping.insert("rmvb", ":/resources/images/movie.png");
    static_iconMapping.insert("flv", ":/resources/images/movie.png");
    static_iconMapping.insert("mov", ":/resources/images/movie.png");
    static_iconMapping.insert("mpg", ":/resources/images/movie.png");
    static_iconMapping.insert("mpeg", ":/resources/images/movie.png");

    static_iconMapping.insert("pdf", ":/resources/images/pdf.png");
    static_iconMapping.insert("chm", ":/resources/images/pdf.png");

    static_iconMapping.insert("mp3", ":/resources/images/music.png");
    static_iconMapping.insert("wma", ":/resources/images/music.png");
    static_iconMapping.insert("ape", ":/resources/images/music.png");
    static_iconMapping.insert("aac", ":/resources/images/music.png");
    static_iconMapping.insert("m4a", ":/resources/images/music.png");

    static_iconMapping.insert("rar", ":/resources/images/rar.png");
    static_iconMapping.insert("tar", ":/resources/images/rar.png");
    static_iconMapping.insert("bz2", ":/resources/images/rar.png");
    static_iconMapping.insert("gz", ":/resources/images/rar.png");
    static_iconMapping.insert("iso", ":/resources/images/rar.png");
    static_iconMapping.insert("img", ":/resources/images/rar.png");
    static_iconMapping.insert("xz", ":/resources/images/rar.png");
    static_iconMapping.insert("zip", ":/resources/images/rar.png");
    static_iconMapping.insert("7z", ":/resources/images/rar.png");

    static_iconMapping.insert("png", ":/resources/images/image.png");
    static_iconMapping.insert("jpg", ":/resources/images/image.png");
    static_iconMapping.insert("jpeg", ":/resources/images/image.png");
    static_iconMapping.insert("gif", ":/resources/images/image.png");
    static_iconMapping.insert("psd", ":/resources/images/image.png");
    static_iconMapping.insert("tif", ":/resources/images/image.png");
    static_iconMapping.insert("tiff", ":/resources/images/image.png");
    static_iconMapping.insert("bmp", ":/resources/images/image.png");

    static_iconMapping.insert("folder", ":/resources/images/bt.png");
}

Thunder::AutoOpen Util::shouldAutoOpen (const QString &fileName)
{
    int idx = fileName.lastIndexOf(".");
    if (idx != -1)
    {
        return static_autoOpenMapping.value(fileName.mid(idx + 1).toLower(), Thunder::Never);
    }

    return Thunder::Never;
}

QString Util::getMD5Hex(const QString &pass)
{
    return QCryptographicHash::hash(pass.toAscii(),
                                    QCryptographicHash::Md5).toHex();
}

QByteArray Util::readWholeFile(const QString &file)
{
    QByteArray result;
    QFile fp (file);
    if (fp.open(QIODevice::ReadOnly))
        result = fp.readAll();

    return result;
}

QString Util::getRandomString(const int &len)
{
    static char alphanum[] = "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "0123456789";

    QString result;
    for (int i = 0; i < len; ++i)
    {
        result.append( alphanum[ rand() % (sizeof(alphanum) - 1) ] );
    }

    return result;
}

QList<QNetworkCookie> Util::parseMozillaCookieFile(const QString &file)
{
    QList<QNetworkCookie> cookies;
    QFile fp (file);
    if (fp.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (! fp.atEnd())
        {
            QByteArray line = fp.readLine();
            if (line.endsWith("\n"))
                line.chop(1);

            QList<QByteArray> fields = line.split('\t');

            if (fields.size() == 7)
            {
                QNetworkCookie cookie;
                QDateTime time;
                time.addSecs(fields.at(4).toULongLong());

                cookie.setDomain(fields.at(0));
                //                cookie.setHttpOnly(fields.at(1));
                cookie.setPath(fields.at(2));
                cookie.setSecure(fields.at(3).toLower() == "true" ? true : false);
                cookie.setExpirationDate(time);
                cookie.setName(fields.at(5));
                cookie.setValue(fields.at(6));

                cookies.append(cookie);
            }
        }
    }

    return cookies;
}

QString Util::getEncryptedPassword(const QString &pass,
                                   const QString &code,
                                   bool encrypted)
{
    QString md5Hash = encrypted ?
                pass :
                QCryptographicHash::hash(pass.toAscii(),
                                         QCryptographicHash::Md5).toHex();

    QString enc = QCryptographicHash::hash(
                md5Hash.toAscii(),
                QCryptographicHash::Md5).toHex();

    return QCryptographicHash::hash(
                enc.toAscii() + code.trimmed().toUpper().toAscii(),
                QCryptographicHash::Md5 ).toHex();
}

bool Util::writeFile(const QString &file, const QByteArray &data)
{
    QFile fp (file);
    if (fp.open(QIODevice::WriteOnly))
        fp.write(data);

    return fp.isOpen();
}

Thunder::File Util::getFileAttr (const QString &fileName, bool isFolder)
{
    Thunder::File file;
    int idx = fileName.lastIndexOf(".");
    if (idx != -1)
    {
        file.extension = fileName.mid(idx + 1);
        file.icon = QIcon (static_iconMapping.value(
                               file.extension.toLower(),
                               ":/resources/images/exe.png"));
    }

    if (isFolder)
        file.icon = QIcon (static_iconMapping.value("folder"));

    return file;
}

QString Util::toReadableSize(const unsigned long long & size)
{
    static const char *labels[] = { "B", "K", "M", "G", "TB", "EB", "PB" };
    size_t i = 0;

    double this_size = size;
    char result[6];
    for (; i < sizeof(labels) / sizeof(char*); ++i)
    {
        if (this_size <= 1024) break;
        this_size /= 1024;
    }
    /// BUG if file too big, >1024PB
    snprintf (result, 6, "%.2f", this_size);

    return QString ("%1 %2").arg(result).arg(labels[i]);
}

QStringList Util::parseFunctionFields(const QByteArray &d)
{
    // chop head (before '(')
    QByteArray data = d;
    data.remove(0, data.indexOf("("));

    // chop last )
    data.chop(1);

    QStringList result;
    bool inQuote = false;
    int field = 1;
    int lastPos = 0;

    for (int i = 0; i < data.size(); ++i)
    {
        if (data.at(i) == ' ' || data.at(i) == '\t')
            continue;
        else if (data.at(i) == '\'')
        {
            // TODO: handle errors
            if (i > 0)
            {
                // handle escape
                if (data.at(i - 1) == '\\')
                {
                    continue;
                }
            }

            if (inQuote)
            {
                result.append(QString::fromUtf8(data.mid(lastPos + 1, i - lastPos - 1)));
                inQuote = false;
            }
            else
            {
                inQuote = true;
                lastPos = i;
            }
        }
        else if (data.at(i) == ',')
        {
            if (! inQuote)
            {
                ++ field;
            }
        }
        else if (! inQuote)
        {
            int j = i;
            while (data.at(j) >= '0' && data.at(j) <= '9' && ++j < data.size());
            if (j != i)
            {
                //                qDebug() << field << data.mid (i, j - i);
                result.append(QString::fromUtf8(data.mid (i, j - i)));
                i = j;
            }
        }
    }

    return result;
}
