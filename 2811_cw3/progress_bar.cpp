#include "progress_bar.h"

void Progress_bar::setBarRange(qint64 max) {
    setRange(0, int(max)); // set the slider's minimum to min and its maximum to max
}

void Progress_bar::setCurrentValue(qint64 current) {
    setValue(int(current)); // hold the slider's current value
}

void Time::setTotalTime(qint64 total_time) {
    this->total = total_time;
}

void Time::setCurrentTime(qint64 current_time) {
    QString lblTime;
    int seconds = int(current_time/1000);
    int t_time = int(total/1000);
    QTime current_formatted((seconds/3600)%60, (seconds/60)%60, seconds%60); // hour, min, sec
    QTime total_formatted((t_time/3600)%60, (t_time/60)%60, t_time%60); // hour, min, sec
    QString format = "mm:ss";
    if(t_time > 3600)
        format = "hh:mm:ss";
    lblTime = current_formatted.toString(format) + "/" + total_formatted.toString(format);

    QLabel::setText(lblTime);
}
