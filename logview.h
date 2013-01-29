#ifndef LOGVIEW_H
#define LOGVIEW_H

#include <QWidget>
#include "thundercore.h"

namespace Ui {
class LogView;
}

class LogView : public QWidget
{
    Q_OBJECT
    
public:
    explicit LogView(QWidget *parent = 0);
    ~LogView();

private slots:
    void logReceived (const QString & body, ThunderCore::ErrorCategory cate);
    
private:
    Ui::LogView *ui;
};

#endif // LOGVIEW_H
