#include <iostream>
#include "TimePoint.h"
using namespace std;

int main() {

    // 1) Создание объектов
    TimePoint now(10, 30, 0);
    cout << "Current time: ";
    now.output();
    cout << " — " << now.timeOfDay() << endl; // определение времени суток(7)

    // 2) Ввод времени
    cout << "\nEnter the alarm time:\n";
    TimePoint alarm;
    alarm.input();

    cout << "The alarm clock is set to: ";
    alarm.output();
    cout << endl;

    // 8) Время до будильника
    TimePoint left = now.timeUntil(alarm);
    cout << "Time until alarm: ";
    left.output();
    cout << endl;

    // 9) Проверка сработает ли будильник сегодня
    if (now.willAlarmRingToday(alarm)) {
        cout << "Alarm will ring today" << endl;
    }
    else {
        cout << "Alarm will ring tomorrow" << endl;
    }

    // 3) Тест аксессоров
    cout << "\nAccessors test:" << endl;
    cout << "Hours: " << now.getHours() << endl;
    cout << "Minutes: " << now.getMinutes() << endl;
    cout << "Seconds: " << now.getSeconds() << endl;

    // Тест суммы/разности временных точек(4-5)
    cout << "\nAddition/subtraction test:" << endl;
    TimePoint t1(10, 30, 0);
    TimePoint t2(2, 45, 30);
    TimePoint sum = t1 + t2;
    TimePoint diff = t1 - t2;
    cout << "t1 + t2 = "; sum.output(); cout << endl;
    cout << "t1 - t2 = "; diff.output(); cout << endl;

    // Тест сравнений(6)
    cout << "\nComparison test:" << endl;
    cout << "t1 == t2: " << (t1 == t2) << endl;
    cout << "t1 < t2: " << (t1 < t2) << endl;
    cout << "t1 > t2: " << (t1 > t2) << endl;

    return 0;
}