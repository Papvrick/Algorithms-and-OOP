#include <iostream>
#include "TimePoint.h"
using namespace std;

int main() {

    // 1) Ввод текущего времени с клавиатуры
    TimePoint now;
    TimePoint anotherNow(10, 3, 0);
    cout << "Enter current time: ";
    cin >> now;
    cout << "Current time: " << now << " — " << now.timeOfDay() << endl;
    cout << "Another time: " << anotherNow << " - " << anotherNow.timeOfDay() << endl;

    // 2) Ввод времени будильника с клавиатуры
    cout << "\nEnter alarm time (hours minutes seconds): ";
    TimePoint alarm;
    cin >> alarm;

    cout << "Alarm set for: " << alarm << endl;

    // 3) Расчет времени до будильника
    TimePoint left = now.timeUntil(alarm);
    cout << "Time until alarm: " << left << endl;

    // 4) Проверка, сработает ли будильник сегодня
    if (now.willAlarmRingToday(alarm)) {
        cout << "Alarm will ring TODAY" << endl;
    }
    else {
        cout << "Alarm will ring TOMORROW" << endl;
    }

    // 5) Тест аксессоров (Геттеры/Сеттеры)
    TimePoint testTime(3, 18, 19);
    cout << "Getters:" << endl;
    cout << "Hours: " << now.getHours() << endl;
    cout << "Minutes: " << now.getMinutes() << endl;
    cout << "Seconds: " << now.getSeconds() << endl;

    cout << "Setters:" << endl;
    testTime.setHours(14);
    testTime.setMinutes(30);
    testTime.setSeconds(20);
    cout << "After changes: " << testTime << endl;

    // 6) Тест суммы/разности временных точек с потоковым вводом
    cout << "\nTest of sum/difference of time points:" << endl;
    TimePoint t1, t2;

    cout << "Enter first time point (hours minutes seconds): ";
    cin >> t1;
    cout << "Enter second time point (hours minutes seconds): ";
    cin >> t2;

    cout << "t1 + t2 = " << (t1 + t2) << endl;
    cout << "t1 - t2 = " << (t1 - t2) << endl;

    // 7) Тест операций сравнения
    cout << "\nComparison test:" << endl;
    cout << "t1 == t2: " << (t1 == t2) << endl;
    cout << "t1 != t2: " << (t1 != t2) << endl;
    cout << "t1 < t2: " << (t1 < t2) << endl;
    cout << "t1 > t2: " << (t1 > t2) << endl;

    return 0;
}