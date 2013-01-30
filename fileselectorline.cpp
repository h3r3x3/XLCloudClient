#include "fileselectorline.h"
#include "ui_fileselectorline.h"

FileSelectorLine::FileSelectorLine(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileSelectorLine),
    isSaveFileDlg(false)
{
    ui->setupUi(this);

    QCompleter *completer = new QCompleter(this);
    QDirModel *dirModel = new QDirModel(completer);
    dirModel->setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    completer->setModel(dirModel);
    ui->lineEdit->setCompleter(completer);
}

QString FileSelectorLine::text()
{
    return ui->lineEdit->text();
}

void FileSelectorLine::setText(const QString &s)
{
    ui->lineEdit->setText(s);
}

void FileSelectorLine::setParam(bool ok, const QString &nameFilter)
{
    isSaveFileDlg = ok;
    this->nameFilter = nameFilter;
}

FileSelectorLine::~FileSelectorLine()
{
    delete ui;
}

void FileSelectorLine::on_toolButton_clicked()
{
    if (isSaveFileDlg)
    {
        const QString & file = QFileDialog::getOpenFileName
                (this, tr("Open file"), QApplication::applicationDirPath(), nameFilter);
        if (! file.isEmpty())
            ui->lineEdit->setText(file);
    }

    else
    {
        const QString & saveFile = QFileDialog::getSaveFileName
                (this, tr("Save to"), QApplication::applicationDirPath(), nameFilter);
        if (! saveFile.isEmpty())
            ui->lineEdit->setText(saveFile);
    }
}
