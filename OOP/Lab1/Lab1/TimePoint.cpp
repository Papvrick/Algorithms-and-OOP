#include "TimePoint.h"
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

void TimePoint::normalize()
{
    if (seconds >= 60) {
        minutes += seconds / 60;
        seconds %= 60;
    }
    else if (seconds < 0) {
        int borrow = (abs(seconds) + 59) / 60;
        minutes -= borrow;
        seconds += borrow * 60;
    }

    if (minutes >= 60) {
        hours += minutes / 60;
        minutes %= 60;
    }
    else if (minutes < 0) {
        int borrow = (abs(minutes) + 59) / 60;
        hours -= borrow;
        minutes += borrow * 60;
    }

    if (hours >= 24) hours %= 24;
    else if (hours < 0) hours = (hours % 24 + 24) % 24;
}

// 1) Конструкторы
TimePoint::TimePoint() : hours(0), minutes(0), seconds(0) {}

TimePoint::TimePoint(int h, int m, int s) {
    if (h >= 0 && h < 24) hours = h; else hours = 0;
    if (m >= 0 && m < 60) minutes = m; else minutes = 0;
    if (s >= 0 && s < 60) seconds = s; else seconds = 0;
}

// 2) Ввод/вывод
void TimePoint::input() {

    cin >> hours >> minutes >> seconds;
    normalize();
}

void TimePoint::output() const {
    cout << setfill('0') << setw(2) << hours << ":"
        << setw(2) << minutes << ":"
        << setw(2) << seconds;
}

// 3) Аксессоры
// Геттеры
int TimePoint::getHours() const { return hours; }
int TimePoint::getMinutes() const { return minutes; }
int TimePoint::getSeconds() const { return seconds; }

// Сеттеры
void TimePoint::setHours(int h) { hours = h; normalize(); }
void TimePoint::setMinutes(int m) { minutes = m; normalize(); }
void TimePoint::setSeconds(int s) { seconds = s; normalize(); }

// 4) Сумма временных точек
TimePoint TimePoint::operator+(const TimePoint& other) const {
    TimePoint result(hours + other.hours,
        minutes + other.minutes,
        seconds + other.seconds);
    result.normalize();
    return result;
}

// 5) Разность временных точек
TimePoint TimePoint::operator-(const TimePoint& other) const {
    TimePoint result(hours - other.hours,
        minutes - other.minutes,
        seconds - other.seconds);
    result.normalize();
    return result;
}

// 6) Сравнения двух временных точек
bool TimePoint::operator==(const TimePoint& other) const {
    return hours == other.hours && minutes == other.minutes && seconds == other.seconds;
}

bool TimePoint::operator!=(const TimePoint& other) const {
    return !(*this == other);
}

bool TimePoint::operator<(const TimePoint& other) const {
    if (hours != other.hours) return hours < other.hours;
    if (minutes != other.minutes) return minutes < other.minutes;
    return seconds < other.seconds;
}

bool TimePoint::operator>(const TimePoint& other) const {
    return other < *this;
}

bool TimePoint::operator<=(const TimePoint& other) const {
    return !(other < *this);
}

bool TimePoint::operator>=(const TimePoint& other) const {
    return !(*this < other);
}

// 7) Время суток 
string TimePoint::timeOfDay() const {
    if (hours >= 6 && hours < 12) {
        return "Morning";
    }
    else if (hours >= 12 && hours < 18) {
        return "Day";
    }
    else if (hours >= 18 && hours < 24) {
        return "Evening";
    }
    else {
        return "Night";
    }
}

// 8) Время до будильника
TimePoint TimePoint::timeUntil(const TimePoint& alarm) const {
    int total1 = hours * 3600 + minutes * 60 + seconds;
    int total2 = alarm.hours * 3600 + alarm.minutes * 60 + alarm.seconds;

    int diff = total2 - total1;
    if (diff < 0) diff += 24 * 3600;

    int h = diff / 3600;
    int m = (diff % 3600) / 60;
    int s = diff % 60;

    return TimePoint(h, m, s);
}

// 9) Проверка сработает ли будильник сегодня
bool TimePoint::willAlarmRingToday(const TimePoint& alarm) const {
    return *this <= alarm; // Если текущее время <= времени будильника, то сработает сегодня
}