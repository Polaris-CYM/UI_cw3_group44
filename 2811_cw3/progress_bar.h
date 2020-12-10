#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include <QSlider>
#include <QLabel>
#include <QTime>

class Progress_bar : public QSlider {
    Q_OBJECT
public:
    Progress_bar(QWidget *parent = nullptr) : QSlider(Qt::Horizontal, parent) {}

public slots:
    void setBarRange(qint64 max); // set the slider's minimum to min and its maximum to max
    void setCurrentValue(qint64 current); // hold the slider's current value
};

class Time : public QLabel {
    Q_OBJECT
public:
    Time(QWidget *parent = nullptr) : QLabel(parent, Qt::Widget) {}

public slots:
    void setTotalTime(qint64 total_time); // set the total time of current video to slider
    void setCurrentTime(qint64 current_time); // set the current time

private:
    qint64 total;
    qint64 current;
};

#endif // PROGRESS_BAR_H
