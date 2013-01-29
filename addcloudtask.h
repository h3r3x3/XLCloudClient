#ifndef ADDCLOUDTASK_H
#define ADDCLOUDTASK_H

#include <QDialog>
#include <QClipboard>
#include <QApplication>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QFileDialog>

#include "thundercore.h"

namespace Ui {
class AddCloudTask;
}

class AddCloudTask : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddCloudTask(ThunderCore *tc, QWidget *parent = 0);
    ~AddCloudTask();

    enum RequestType
    {
        AddSingleTaskPre,
        AddSingleTask,
        AddBatchTaskPre,
        AddBatchTask
    };

    QString getSingleTaskURL ();
    void setSingleTaskInfo (Thunder::RemoteTask remote_task);
    
private slots:
    void on_url_textChanged(const QString &arg1);

    void slotRemoteTaskChanged (ThunderCore::RemoteTaskType type);

    void on_buttonBox_accepted();

    void on_uploadBTFile_clicked();

protected:
    void showEvent(QShowEvent *e);

private:
    Ui::AddCloudTask *ui;

    ThunderCore *tcore;

    Thunder::RemoteTask tmp_singleRemoteTask;

    QStandardItemModel *bt_model;

signals:
    void requestChanged (AddCloudTask::RequestType type);

};

#endif // ADDCLOUDTASK_H
