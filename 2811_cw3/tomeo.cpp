/*
 *
 *    ______
 *   /_  __/___  ____ ___  ___  ____
 *    / / / __ \/ __ `__ \/ _ \/ __ \
 *   / / / /_/ / / / / / /  __/ /_/ /
 *  /_/  \____/_/ /_/ /_/\___/\____/
 *              video for sports enthusiasts...
 *
 *  2811 cw3 : twak
 */

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
#include <QLabel>

using namespace std;

// read in videos and thumbnails to this directory
vector<TheButtonInfo> getInfoIn (string loc) {

    vector<TheButtonInfo> out =  vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    while (it.hasNext()) { // for all files

        QString f = it.next();

            if (f.contains("."))

#if defined(_WIN32)
            if (f.contains(".wmv"))  { // windows
#else
            if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif

            QString thumb = f.left( f .length() - 4) +".png";
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
                    }
                    else
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
        }
    }

    return out;
}


int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn( string(argv[1]) );

    if (videos.size() == 0) {

        const int result = QMessageBox::question(
                    NULL,
                    QString("Tomeo"),
                    QString("no videos found! download, unzip, and add command line argument to \"quoted\" file location. Download videos from Tom's OneDrive?"),
                    QMessageBox::Yes |
                    QMessageBox::No );

        switch( result )
        {
        case QMessageBox::Yes:
          QDesktopServices::openUrl(QUrl("https://leeds365-my.sharepoint.com/:u:/g/personal/scstke_leeds_ac_uk/EcGntcL-K3JOiaZF4T_uaA4BHn6USbq2E55kF_BTfdpPag?e=n1qfuN"));
          break;
        default:
            break;
        }
        exit(-1);
    }

    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);

    // a row of buttons
    QWidget *buttonWidget = new QWidget();

    // the layout of scroll area
    QVBoxLayout *right = new QVBoxLayout;
    right->setSpacing(10);

    // the layout of the window
    QGridLayout *windowLayout = new QGridLayout();

    // a list of the buttons
    vector<TheButton*> buttons;


    // create the four buttons
    for ( int i = 0; i <6; i++ ) {
        TheButton *button = new TheButton(buttonWidget);
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* ))); // when clicked, tell the player to play.
        buttons.push_back(button);
        // when the cursor is hovering over the thumbnail, the cursor shape changes to a hand.
        button->setCursor(Qt::PointingHandCursor);
        // set the minimumSize of each thumbnail widget
        button->setMinimumSize(160, 90);
        // set the margin of each thumbnail to 0
        button->setContentsMargins(0,0,0,0);
        // add the buttonWidgets to the QVBoxLayout
        right->addWidget(button);

        button->init(&videos.at(i));

    }

    //buttonWidget->setFixedSize(125, 7500);
    buttonWidget->setMinimumSize(QSize(160, 750));

    QScrollArea *scrollArea = new QScrollArea();
    //scrollArea->setMaximumWidth(500);
    scrollArea->setFixedWidth(220);

    scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scrollArea->setWidgetResizable( true );

    buttonWidget->setLayout(right);

    // set buttonWidget to the scrollArea
    scrollArea->setWidget(buttonWidget);

    // start from row 0 column 0 and occupy 1 row and 5 columns
    windowLayout->addWidget(videoWidget, 0, 0, 1, 5);

    windowLayout->addWidget(scrollArea, 0, 6, 0, 1);

    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);
    QWidget * p_wdg = new QWidget();
    QPushButton *pause_play = new QPushButton(p_wdg);
    pause_play->setText("⏯");
    pause_play->setCheckable(true);
    pause_play->connect(pause_play, SIGNAL(toggled(bool)), player, SLOT (pausePlay(bool)));
    //scrollArea->setWidget(button);
    windowLayout->addWidget(p_wdg, 6, 2, 10, 0); // cycle 1

    // create the main window and layout
    QWidget window;


    window.setLayout(windowLayout);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(750, 500);

    // showtime!

    window.show();

    // wait for the app to terminate
    return app.exec();
}
