#ifndef SIMPLEEDITOR_H
#define SIMPLEEDITOR_H

#include <QDialog>

namespace Ui {
class SimpleEditor;
}

class SimpleEditor : public QDialog
{
    Q_OBJECT
    
public:
    explicit SimpleEditor(QWidget *parent = 0);
    ~SimpleEditor();

    QString getText ();
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::SimpleEditor *ui;
    bool isAccepted;
};

#endif // SIMPLEEDITOR_H
