//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H


#include <QApplication>
#include <QMediaPlayer>
#include "the_button.h"
#include "progress_bar.h"
#include <vector>
#include <QTimer>

using namespace std;

class ThePlayer : public QMediaPlayer {

Q_OBJECT

private:
    vector<TheButtonInfo>* infos;
    vector<TheButton*>* buttons;
    QTimer* mTimer;
    long updateCount = 0;
    QPushButton *pause_play_button;
    QSlider *sound_bar;
    QPushButton *mute_button;
    int current_index;

public:
    ThePlayer() : QMediaPlayer(NULL) {
        connect (this, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (playStateChanged(QMediaPlayer::State)) );

        mTimer = new QTimer(NULL);
        mTimer->setInterval(1000); // 1000ms is one second between ...
        mTimer->start();
        connect( mTimer, SIGNAL (timeout()), SLOT ( shuffle() ) ); // ...running shuffle method
    }

    // all buttons have been setup, store pointers here
    void setContent(vector<TheButton*>* b, vector<TheButtonInfo>* i);

    // make the state of pause-play button can be changed in ThePlayer
    void setPausePlayButton(QPushButton *);
    void setSoundBar(QSlider *);
    void setMuteButton(QPushButton *);

private slots:

    // change the image and video for one button every one second

    void playStateChanged (QMediaPlayer::State ms);

public slots:

    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);
    void pausePlay (bool); // pause/play the video
    void mute(bool); // mute the video
    void replay(); // replay the current video
    void soundBar(int); // event when drag the sound bar
    void getTotalTime(qint64); // get the total time of the current video
    void setPlayPosition(int); // set the playback position of the current video
    void toPrevious(); // switch to the previous video
    void toNext(); // switch to the next video
};

#endif //CW2_THE_PLAYER_H
