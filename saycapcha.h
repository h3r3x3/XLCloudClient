#ifndef SAYCAPCHA_H
#define SAYCAPCHA_H

#include <QDialog>
#include <QPixmap>

namespace Ui {
class SayCapcha;
}

class SayCapcha : public QDialog
{
    Q_OBJECT
    
public:
    explicit SayCapcha(QWidget *parent = 0);
    ~SayCapcha();

    void setImage (const QByteArray & data);

private slots:
    void on_code_returnPressed();

    void accept();
    void reject();

signals:
    void capchaReady (const QString & capcha);

private:
    Ui::SayCapcha *ui;
};

#endif // SAYCAPCHA_H
