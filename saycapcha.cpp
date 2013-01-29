#include "saycapcha.h"
#include "ui_saycapcha.h"

SayCapcha::SayCapcha(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SayCapcha)
{
    ui->setupUi(this);
}

void SayCapcha::setImage(const QByteArray &data)
{
    ui->capcha->setPixmap(QPixmap::fromImage(QImage::fromData(data)));
}

SayCapcha::~SayCapcha()
{
    delete ui;
}

void SayCapcha::on_code_returnPressed()
{
    accept();
}

void SayCapcha::accept()
{
    emit capchaReady(ui->capcha->text());

    close ();
}

void SayCapcha::reject()
{
    close ();
}
