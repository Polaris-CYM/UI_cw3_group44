//
// Created by twak on 11/11/2019.
//

#include "the_player.h"

using namespace std;

// all buttons have been setup, store pointers here
void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;
    jumpTo(buttons -> at(0) -> info);
}

//OLD: change the image and video for one button every one second
//UPDATED (iteration 1): Videos no longer shuffle

void ThePlayer::playStateChanged (QMediaPlayer::State ms) {
    switch (ms) {
        case QMediaPlayer::State::StoppedState:

            break;
    default:
        break;
    }
}

void ThePlayer::jumpTo (TheButtonInfo* button) {
    setMedia( * button -> url);
    play();
}

void ThePlayer::pausePlay (bool checked) {
    if(checked) { // if the button is checked, the video should be paused
       pause();
   }
   else { // else the video should be played
       play();
   }
}

void ThePlayer::mute (bool checked) {
   if(checked) { // if the mute button is checked, set the volumn and value of the sound-bar to 0
       setVolume(0);
       sound_bar->setValue(0);
   }
   else { //else set the volumn and the value of the sound-bar to 100
       setVolume(100);
       sound_bar->setValue(100);
   }
}

void ThePlayer::getTotalTime(qint64 total_time) {
    total_time = this->duration(); // get the duration of the currect video
}

void ThePlayer::setPlayPosition(int position) {
    QMediaPlayer::setPosition(qint64(position));
}

void ThePlayer::setPausePlayButton(QPushButton *btnPausePlay) {
    pause_play_button = btnPausePlay;
}

void ThePlayer::replay() {
    stop();
    play();
    pause_play_button->setChecked(false);
}

void ThePlayer::setSoundBar(QSlider *sound_Bar) {
    sound_bar = sound_Bar;
}

void ThePlayer::setMuteButton(QPushButton *btnMute) {
    mute_button = btnMute;
}

void ThePlayer::soundBar(int value) {
    setVolume(value);
    if(sound_bar->value()==0) {
        mute_button->setChecked(true);
    }
    else {
        mute_button->setChecked(false);
    }
}

void ThePlayer::toPrevious() {
    current_index = (current_index - 1) % buttons->size(); // locate to the previous video
    setMedia(*infos->at(current_index).url);
    play();
}

void ThePlayer::toNext() {
    current_index = (current_index + 1) % buttons->size(); // locate to the next video
    setMedia(*infos->at(current_index).url);
    play();
}
