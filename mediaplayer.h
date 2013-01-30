#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <phonon/audiooutput.h>
#include <phonon/seekslider.h>
#include <phonon/mediaobject.h>
#include <phonon/volumeslider.h>
#include <phonon/backendcapabilities.h>

class MediaPlayer;
extern MediaPlayer *mediaPlayer;

class MediaPlayer : public QObject
{
    Q_OBJECT
public:
    void static init ()
    { mediaPlayer = new MediaPlayer(); }

    explicit MediaPlayer(QObject *parent = 0);
    ~MediaPlayer();

    void play(const QString &filepath);
    bool isPlaying();

    void stop();
    void setVolume (int vol);

    Phonon::MediaObject *mediaObject;
    Phonon::AudioOutput *audioOutput;

signals:

public slots:

};

#endif // MEDIAPLAYER_H
