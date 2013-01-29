#include "transf0r.h"
#include "ui_transf0r.h"

Transf0r::Transf0r(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Transf0r),
    my_storagePath("/dev/shm/")
{
    ui->setupUi(this);
}

Transf0r::~Transf0r()
{
    delete ui;
}

void Transf0r::addCloudTask(const Thunder::RemoteTask &taskInfo)
{
    QListWidgetItem *item = new QListWidgetItem;
    ui->listWidget->addItem(item);

    DownloaderChildWidget *cw = new DownloaderChildWidget (item,
                                                           taskInfo.url,
                                                           taskInfo.name,
                                                           my_storagePath, this);
    connect(cw, SIGNAL(ItemDeleted(int)), SLOT(slotItemCanDelete(int)));

    item->setSizeHint(cw->sizeHint());
    ui->listWidget->setItemWidget(item, cw);
}

void Transf0r::slotItemCanDelete(int id)
{
    delete ui->listWidget->takeItem(id - 1);
}
