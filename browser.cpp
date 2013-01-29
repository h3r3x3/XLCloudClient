#include "browser.h"
#include "ui_browser.h"

Browser::Browser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Browser)
{
    ui->setupUi(this);
}

Browser::~Browser()
{
    delete ui;
}

void Browser::on_url_returnPressed()
{
    QString url = ui->url->text();
    if (! url.startsWith("http://"))
        url.prepend("http://");

    ui->webView->load(url);
}

void Browser::on_webView_urlChanged(const QUrl &arg1)
{
    ui->url->setText(arg1.toString());
}

void Browser::on_reload_clicked()
{
    ui->webView->reload();
}

void Browser::on_fwd_clicked()
{
    ui->webView->forward();
}

void Browser::on_stop_clicked()
{
    ui->webView->stop();
}

void Browser::on_webView_linkClicked(const QUrl &arg1)
{
    qDebug() << "Clicked on: " << arg1;
    ui->webView->load(arg1);
}

void Browser::on_search_returnPressed()
{
    switch (ui->comboBox->currentIndex())
    {
    case 0:
        ui->webView->load(QString("http://www.verycd.com/search/entries/%1")
                          .arg(ui->search->text()));
        break;
    case 1:
        ui->webView->load(QString("http://yyets.com/?mod=2&ac=search_result"
                                  "&op=normal&class=all&keyword=%1")
                          .arg(ui->search->text()));
        break;
    }
}
