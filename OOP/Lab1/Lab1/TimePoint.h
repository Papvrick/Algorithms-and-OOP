#pragma once
#include <string>
using namespace std;

class TimePoint {
private:
    int hours;
    int minutes;
    int seconds;

    void normalize(); // вспомогательная функция

public:
    // 1) Конструкторы
    TimePoint();
    TimePoint(int h, int m = 0, int s = 0);

    // 2) Ввод / вывод
    void input();
    void output() const;

    // 3) Аксессоры
    // Геттеры
    int getHours() const;
    int getMinutes() const;
    int getSeconds() const;

    // Сеттеры
    void setHours(int h);
    void setMinutes(int m);
    void setSeconds(int s);

    // 4) Сумма временных точек
    TimePoint operator+(const TimePoint& other) const;

    // 5) Разность временных точек
    TimePoint operator-(const TimePoint& other) const;

    // 6) Сравнение двух временных точек
    bool operator==(const TimePoint& other) const;
    bool operator!=(const TimePoint& other) const;
    bool operator<(const TimePoint& other) const;
    bool operator>(const TimePoint& other) const;
    bool operator<=(const TimePoint& other) const;
    bool operator>=(const TimePoint& other) const;

    // 7) Время суток
    string timeOfDay() const;

    // 8) Время до будильника
    TimePoint timeUntil(const TimePoint& alarm) const;

    // 9) Проверка сработает ли будильник сегодня
    bool willAlarmRingToday(const TimePoint& alarm) const;
};