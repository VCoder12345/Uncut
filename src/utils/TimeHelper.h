#pragma once
#include <cstdint>
#include <chrono>
#include <QString>

struct TimeObj {
	int hours, minutes, seconds, ms;

    QString toHHMMSS() {
        return QString::asprintf("%02d:%02d:%02d", hours, minutes, seconds);
    }

    static TimeObj timeFromMicroseconds(int64_t mics) {
        TimeObj obj;
        int64_t totalMilliseconds = mics / 1000;
        obj.hours = totalMilliseconds / (1000 * 60 * 60);
        obj.minutes = (totalMilliseconds / (1000 * 60)) % 60;
        obj.seconds = (totalMilliseconds / 1000) % 60;
        obj.ms = totalMilliseconds % 1000;
        /*int microseconds = (ns / 1000) % 1000;
        int remainingNanoseconds = ns % 1000;*/

        return obj;
    }

};
