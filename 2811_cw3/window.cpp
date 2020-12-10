#include "window.h"

// read in videos and thumbnails to this directory
vector<TheButtonInfo> Window::getInfoIn (string loc) {

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
                        // voodoo to create an icon for the button
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite));
                        // convert the file location to a generic url
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f ));
                        // add to the output list
                        out . push_back(TheButtonInfo( url , ico  ) );
                    }
                    else
                        qDebug() << "warning: skipping video because I couldn't process thumbnail "
                                 << thumb << endl;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail "
                         << thumb << endl;
        }
    }

    return out;
}


void Window::getVideo(int argc, char **argv) {
    if (argc == 2)
        videos = getInfoIn( string(argv[1]) );

    if (videos.size() == 0) {

        const int result = QMessageBox::question(
                    NULL,
                    QString("Tomeo"),
                    QString("no videos found! download, unzip, and add command line argument"
                            " to \"quoted\" file location. Download videos from Tom's OneDrive?"),
                    QMessageBox::Yes |
                    QMessageBox::No );

        switch( result )
        {
        case QMessageBox::Yes:
          QDesktopServices::openUrl(QUrl("https://leeds365-my.sharepoint.com/:u:/g/personal/"
                                         "scstke_leeds_ac_uk/"
                                         "EcGntcL-K3JOiaZF4T_uaA4BHn6USbq2E55kF_BTfdpPag?e="
                                         "n1qfuN"));
          break;
        default:
            break;
        }
        exit(-1);
    }
}

void Window::setWindowLayout() {
    this->setPlayerArea();
    this->setScrollArea();
    this->setMuteButton();
    this->setSoundBar();
    this->setPauseButton();
    this->setReplayButton();
    this->setProgressBar();


}

void Window::setPlayerArea() {
    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;
    // the QMediaPlayer which controls the playback
    player = new ThePlayer;
    player->setVideoOutput(videoWidget);

    // start from row 0 column 0 and occupy 1 row and 5 columns
    this->addWidget(videoWidget, 0, 0, 4, 5);
}

void Window::setScrollArea() {

    QWidget *buttonWidget = new QWidget(); // a row of buttons

    buttons = new vector<TheButton*>(); // a list of the buttons

    // the layout of scroll area
    QVBoxLayout *right = new QVBoxLayout();
    right->setSpacing(10);

    buttonWidget->setLayout(right);
    buttonWidget->setMinimumSize(QSize(150, 750)); //set the size of the widget in scroll area

    // create the buttons
    for ( int i = 0; i <6; i++ ) {
        TheButton *button = new TheButton(buttonWidget);
        // when clicked, tell the player to play.
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )),
                        player, SLOT (jumpTo(TheButtonInfo* )));
        buttons->push_back(button);
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

    // tell the player what buttons and videos are available
    player->setContent(buttons, &videos);


    // create a new scroll area
    scrollArea = new QScrollArea();
    scrollArea->setFixedWidth(220);

    scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );


    scrollArea->setWidget(buttonWidget); // set buttonWidget to the scrollArea
    this->addWidget(scrollArea, 0, 6, 20, 1); // set the scroll area to the window

}

void Window::setPauseButton() {
    // tell the player what buttons and videos are available
    player->setContent(buttons, &videos);
    QWidget *p_wdg = new QWidget();
    pause_play = new QPushButton(p_wdg);

    player->setPausePlayButton(pause_play);

    pause_play->setFixedSize(100, 45);
    // the cursor shape changes to a hand when hovering over the pause/play button
    pause_play->setCursor(Qt::PointingHandCursor);
    pause_play->setText("⏯");
    pause_play->setCheckable(true);
    pause_play->connect(pause_play, SIGNAL(toggled(bool)), player, SLOT (pausePlay(bool)));
    this->addWidget(p_wdg, 6, 2, 9, 1); // set the pause/play button to the window
}

void Window::setMuteButton() {
    // Mute button widget - cycle 2
    QWidget * m_wdg = new QWidget();
    mute_b = new QPushButton(m_wdg);

    player->setMuteButton(mute_b);

    mute_b->setFixedSize(100, 45);
    // the cursor shape changes to a hand when hovering over the mute button
    mute_b->setCursor(Qt::PointingHandCursor);
    mute_b->setText("🔇");
    mute_b->setCheckable(true);
    mute_b->connect(mute_b, SIGNAL(toggled(bool)), player, SLOT (mute(bool)));
    this->addWidget(m_wdg, 6, 0, 9, 1); // set the mute button to the window
}


void Window::setReplayButton() {
    // Stop widget
    QWidget * st_wdg = new QWidget();
    replay = new QPushButton(st_wdg);
    replay->setFixedSize(100, 45);
    // the cursor shape changes to a hand when hovering over the stop button
    replay->setCursor(Qt::PointingHandCursor);
    replay->setText("🔁");
    replay->setCheckable(false);
    replay->connect(replay, SIGNAL(clicked()), player, SLOT (replay()));
    this->addWidget(st_wdg, 6, 4, 9, 1); // set the stop button to the window
}

void Window::setProgressBar() {
    QWidget *progressBar = new QWidget();
    QHBoxLayout *bar_layout = new QHBoxLayout(); // create a new layout for progress bar

    // create the progress bar and time label
    progress_bar = new Progress_bar();
    slider_time = new Time();

    connect(player, SIGNAL(durationChanged(qint64)), progress_bar, SLOT(setBarRange(qint64)));
    connect(player, SIGNAL(durationChanged(qint64)), slider_time, SLOT(setTotalTime(qint64)));
    connect(progress_bar, SIGNAL(valueChanged(int)), player, SLOT(setPlayPosition(int)));
    connect(player, SIGNAL(positionChanged(qint64)), progress_bar, SLOT(setCurrentValue(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), slider_time, SLOT(setCurrentTime(qint64)));

    progress_bar->setCursor(Qt::PointingHandCursor);
    bar_layout->addWidget(progress_bar);
    bar_layout->addWidget(slider_time);
    progressBar->setLayout(bar_layout);

    this->addWidget(progressBar, 4, 0, 2, 5); // set the progress bar to the window
}

void Window::setSoundBar() {
    QWidget *s_wdg = new QWidget();
    sound_bar = new QSlider(Qt::Horizontal, s_wdg);
    player->setSoundBar(sound_bar);
    sound_bar->setRange(0, 100);
    sound_bar->setFixedWidth(100);
    sound_bar->setValue(100);
    sound_bar->connect(sound_bar, SIGNAL(valueChanged(int)), player, SLOT(soundBar(int)));

    sound_bar->setCursor(Qt::PointingHandCursor);

    this->addWidget(s_wdg, 14, 0, 3, 2);
}

