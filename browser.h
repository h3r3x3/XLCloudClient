#ifndef BROWSER_H
#define BROWSER_H

#include <QWidget>
#include <QUrl>
#include <QDebug>

namespace Ui {
class Browser;
}

class Browser : public QWidget
{
    Q_OBJECT
    
public:
    explicit Browser(QWidget *parent = 0);
    ~Browser();
    
private slots:
    void on_url_returnPressed();
    void on_webView_urlChanged(const QUrl &arg1);
    void on_reload_clicked();

    void on_fwd_clicked();
    void on_stop_clicked();

    void on_webView_linkClicked(const QUrl &arg1);

    void on_search_returnPressed();

private:
    Ui::Browser *ui;
};

#endif // BROWSER_H
