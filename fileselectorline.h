#ifndef FILESELECTORLINE_H
#define FILESELECTORLINE_H

#include <QWidget>
#include <QCompleter>
#include <QDirModel>
#include <QFileDialog>

namespace Ui {
class FileSelectorLine;
}

class FileSelectorLine : public QWidget
{
    Q_OBJECT
    
public:
    explicit FileSelectorLine(QWidget *parent = 0);
    ~FileSelectorLine();

    /*!
     * \brief set file selector widget parameter
     * \param If true, it's a saveFileDialog otherwise it wold be a openFileDialog
     * \param Filter on file names
     */
    void setParam (bool ok, const QString & nameFilter);

    /*!
     * \brief Get the text in the lineedit
     * \return
     */
    QString text();

    /*!
     * \brief Set the text of the line editor
     * \param s
     */
    void setText (const QString & s);

private slots:
    void on_toolButton_clicked();

private:
    Ui::FileSelectorLine *ui;

    /*!
     * \brief isSaveFileDlg
     */
    bool isSaveFileDlg;
    QString nameFilter;
};

#endif // FILESELECTORLINE_H
