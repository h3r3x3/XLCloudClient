#ifndef VIDEOPANEL_H
#define VIDEOPANEL_H

#include <QWidget>
#include <QStatusBar>
#include "qmpwidget.h"

namespace Ui {
class VideoPanel;
}

class VideoPanel : public QWidget
{
    Q_OBJECT
    
public:
    explicit VideoPanel(QWidget *parent = 0);
    ~VideoPanel();
    
    void play (const QString & url);
    void setStatusBar (QStatusBar *bar);

private slots:
    void on_stopButton_clicked();
    void on_muteButton_clicked();

    void slotMplayerStatusChanged (int);

    void on_playPauseButton_clicked();

private:
    Ui::VideoPanel *ui;

    int m_storedVolume;
};

#endif // VIDEOPANEL_H
