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
#include <QDebug>
#include "the_player.h"
#include "the_button.h"
#include "window.h"
#include <QtGui>
#include <QWidget>

using namespace std;

int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // create the main window and layout
    QWidget window;
    Window *windowLayout = new Window(&window);
    windowLayout->getVideo(argc, argv);
    windowLayout->setWindowLayout();
    window.setLayout(windowLayout);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(750, 500);

    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
