#include "simpleeditor.h"
#include "ui_simpleeditor.h"

SimpleEditor::SimpleEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimpleEditor),
    isAccepted(false)
{
    ui->setupUi(this);
}

SimpleEditor::~SimpleEditor()
{
    delete ui;
}

QString SimpleEditor::getText()
{
    return isAccepted ? ui->plainTextEdit->toPlainText() : QString ();
}

void SimpleEditor::on_buttonBox_accepted()
{
    isAccepted = true;
}
