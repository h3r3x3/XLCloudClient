#include "videopanel.h"
#include "ui_videopanel.h"

VideoPanel::VideoPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoPanel)
{
    ui->setupUi(this);
    ui->mplayer->setSeekSlider(ui->seekSlider);
    ui->mplayer->setVolumeSlider(ui->volumeSlider);

    connect (ui->mplayer, SIGNAL(stateChanged(int)),
             SLOT(slotMplayerStatusChanged(int)));
}

VideoPanel::~VideoPanel()
{
    delete ui;
}

void VideoPanel::slotMplayerStatusChanged(int s)
{
    if (s == MPlayer::PlayingState)
    {
        ui->playPauseButton->setIcon(
                    QIcon(":/resources/images/media-playback-pause.png"));
    }
    else
    {
        ui->playPauseButton->setIcon(
                    QIcon(":/resources/images/media-playback-start.png"));
    }
}

void VideoPanel::setStatusBar(QStatusBar *bar)
{
    ui->mplayer->setStatusBar(bar);
}

void VideoPanel::play(const QString &url)
{
    ui->mplayer->load(url);
}

void VideoPanel::on_stopButton_clicked()
{
    ui->mplayer->stop();
}

void VideoPanel::on_muteButton_clicked()
{
    if (! ui->muteButton->isChecked())
        ui->volumeSlider->setValue(m_storedVolume);
    else
    {
        m_storedVolume = ui->volumeSlider->value();
        ui->volumeSlider->setValue(0);
    }
}

void VideoPanel::on_playPauseButton_clicked()
{
    if (ui->mplayer->state() == ui->mplayer->PlayingState)
        ui->mplayer->stop();
    else
    {
        //TODO: load last video!
        ui->mplayer->play();
    }
}
