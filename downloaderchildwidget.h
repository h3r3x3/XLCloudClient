#ifndef DOWNLOADERCHILDWIDGET_H
#define DOWNLOADERCHILDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QDesktopServices>
#include <QTimer>
#include <QMessageBox>
#include <QMenu>
#include <QClipboard>
#include <QDir>
#include <QListWidgetItem>
#include <QFileIconProvider>
#include <QAction>
#include <QKeyEvent>
#include <QFile>
#include <QNetworkCookie>
#include <QUrl>

#include "downloader.h"
#include "util.h"

namespace Ui {
class DownloaderChildWidget;
}

class DownloaderChildWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DownloaderChildWidget(QListWidgetItem *item,
                                   const QString & downloadUrl,
                                   const QString & fileName,
                                   const QString & folderName,
                                   QWidget *parent = 0);
    ~DownloaderChildWidget();

    void copyURLtoClipboard (const QString & txt);

    QString m_fileName, m_folderName, m_url;
    double m_percentage;
    QListWidgetItem *m_item; // row in list widget

    QSize sizeHint();

signals:
    void ItemDeleted (int);
    
private:
    Ui::DownloaderChildWidget *ui;
    Downloader m_Downloader;
    QTimer m_taskStatusRoutineTimer;

    bool question (const QString & msg);

protected:
    void paintEvent(QPaintEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);

private slots:
    void taskStatusChanged (Downloader::TaskStatusX ts);
    void getCurrentTaskStatus ();

    void on_openFileLabel_linkActivated(const QString &link);
    void on_openFolderLabel_linkActivated(const QString &link);
    void on_trashTaskBtn_clicked();
};

#endif // DOWNLOADERCHILDWIDGET_H
