#include "logview.h"
#include "ui_logview.h"

LogView::LogView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogView)
{
    ui->setupUi(this);
}

LogView::~LogView()
{
    delete ui;
}

void LogView::logReceived(const QString &body, ThunderCore::ErrorCategory cate)
{
    QString str;

    switch (cate)
    {
    case ThunderCore::Info:
        str.append("[INFO] ");
        break;
    case ThunderCore::Notice:
        str.append("[NOTICE] ");
        break;
    case ThunderCore::Warning:
        str.append("[WARNING] ");
        break;
    case ThunderCore::Critical:
        str.append("[CRITICAL] ");
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    str.append(body);
    ui->textBrowser->append(str);
}
