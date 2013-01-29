#ifndef THUNDERPANEL_H
#define THUNDERPANEL_H

#include <QWidget>
#include <QStandardItemModel>
#include <QMenu>
#include <QAction>
#include <QCursor>
#include <QApplication>
#include <QClipboard>
#include <QStandardItem>
#include <QDebug>

#include "CloudObject.h"
#include "util.h"

namespace Ui {
class ThunderPanel;
}

class ThunderPanel : public QWidget
{
    Q_OBJECT
    
public:
    enum RequestType
    {
        Download,
        Preview
    };

    enum IndirectRequestType
    {
        RemoveTasks,
        AddTask
    };

    explicit ThunderPanel(QWidget *parent = 0);
    ~ThunderPanel();

    void setCloudTasks (const QList<Thunder::Task> & tasks);
    QStringList getSelectedTaskIDs ();

    Thunder::RemoteTask getFirstSelectedTask ();
    
signals:
    void doThisLink (const Thunder::RemoteTask & task, ThunderPanel::RequestType type);
    void doIndirectRequest (ThunderPanel::IndirectRequestType type);

private:
    Ui::ThunderPanel *ui;

    QStandardItemModel *my_model;
    QString getUserDataByOffset (unsigned long long offset, int row = -1);

    QMenu *my_contextMenu;

private slots:
    void slotShowContextMenu (const QPoint & pt);

    void slotPreviewThisTask ();
    void slotDownloadThisTask ();

    void slotRemoveTheseTasks ();

    void slotCopyDownloadAddress ();
    void slotCopySourceAddress ();
};

#endif // THUNDERPANEL_H
