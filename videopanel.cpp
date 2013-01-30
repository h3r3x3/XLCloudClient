/*
 *  CloudClient - A Qt cloud client for lixian.vip.xunlei.com
 *  Copyright (C) 2012 by Aaron Lewis <the.warl0ck.1989@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
        //TODO: load last video? perhaps.
        ui->mplayer->play();
    }
}
