#ifndef TRANSF0R_H
#define TRANSF0R_H

#include <QWidget>
#include <QListWidgetItem>

#include "CloudObject.h"
#include "downloaderchildwidget.h"

namespace Ui {
class Transf0r;
}

class Transf0r : public QWidget
{
    Q_OBJECT
    
public:
    explicit Transf0r(QWidget *parent = 0);
    ~Transf0r();

    void setStoragePath (const QString & path);
    void addCloudTask (const Thunder::RemoteTask & taskInfo);
    
private slots:
    void slotItemCanDelete (int id);

private:
    Ui::Transf0r *ui;

    QString my_storagePath;
};

#endif // TRANSF0R_H
