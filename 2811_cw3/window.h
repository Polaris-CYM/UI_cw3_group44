#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include "the_player.h"
#include "the_button.h"
#include <QtGui>
#include <QtWidgets/QScrollArea>
#include <QVBoxLayout>
#include "the_player.h"
#include "the_button.h"


class Window : public QGridLayout
{
    Q_OBJECT
private:
    vector<TheButtonInfo> getInfoIn(string);  // read in videos and thumbnails to this directory
    void setPlayerArea();         // set the layout of video player
    void setScrollArea();         // set the layout of scroll area
    void setPauseButton();  // set the layout of the pause/play button
    void setStopButton();   //set layout of stop button
    void setMuteButton();   //set layout of mute button

    QWidget *window;               // the widget of window
    vector<TheButtonInfo> videos;  // collect all the videos in the folder
    ThePlayer *player;             // the QMediaPlayer which controls the playback
    vector<TheButton*> *buttons;   // a list of the buttons
    QScrollArea *scrollArea;       // the scroll area
    QPushButton *pause_play;       // the pause/play button


public:
    Window(QWidget *parent) : QGridLayout(parent)
    {
        this->window = parent;
    }
    ~Window() {}

    void getVideo(int argc, char *argv[]);  // get all the videos in the "videos" folder
    void setWindowLayout();  // set the layout of the widget in the window
};


#endif // WINDOW_H
