#include <iostream>
#include "dynamicarray.h"
#include <Windows.h>
using namespace std;

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // Конструктор из обычного массива
    cout << "Конструктор из обычного массива:" << endl;
    int testData[] = { 1, 2, 3, 4, 5 };
    DynamicArray testArr(testData, 5);
    cout << testArr << endl;

    // Конструктор перемещения
    cout << "\nТестирование конструктора перемещения:" << endl;
    DynamicArray sourceArr;
    cout << "Введите элементы для исходного массива:\n";
    cin >> sourceArr;
    cout << "Исходный массив: " << sourceArr << endl;

    DynamicArray movedArr(move(sourceArr));
    cout << "Перемещенный массив: " << movedArr << endl;
    cout << "Исходный массив после перемещения: " << sourceArr << endl;

    // Присваивание перемещением
    cout << "\nТестирование присваивания перемещением:" << endl;
    DynamicArray sourceArr2;
    cout << "Введите элементы для второго исходного массива:\n";
    cin >> sourceArr2;
    cout << "Второй исходный массив: " << sourceArr2 << endl;

    DynamicArray assignedArr;
    assignedArr = move(sourceArr2);
    cout << "Массив после присваивания перемещением: " << assignedArr << endl;
    cout << "Второй исходный массив после присваивания: " << sourceArr2 << endl;

    DynamicArray arr1, arr2;

    int val, idx;

    cout << " Первый массив:\n";
    cin >> arr1;

    cout << "\nВторой массив:\n";
    cin >> arr2;

    cout << "\nПервый массив: " << arr1 << endl;
    cout << "Второй массив: " << arr2 << endl;

    // Конструктор копирования
    DynamicArray arrCopy(arr1);;
    cout << "Копия первого массива: " << arrCopy << endl;

    // Оператор присваивания
    DynamicArray arrAssigned;
    arrAssigned = arr2;
    cout << " Оператор присваивания для второго массива: " << arrAssigned << endl;

    // Операторы сравнения
    cout << "\nОператоры сравнения :" << endl;
    cout << "arr1 == arr2: " << (arr1 == arr2 ? "да" : "нет") << endl;
    cout << "arr1 != arr2: " << (arr1 != arr2 ? "да" : "нет") << endl;
    cout << "arr1 == arrCopy: " << (arr1 == arrCopy ? "да" : "нет") << endl;

    // Функция find
    cout << "Введите значение для поиска в первом массиве: ";
    cin >> val;
    int foundIndex = arr1.find(val);
    if (foundIndex != -1) {
        cout << "Элемент " << val << " найден по индексу: " << foundIndex << endl;
    }
    else {
        cout << "Элемент " << val << " не найден в первом массиве" << endl;
    }

    // Вставка элемента
    cout << "\nВведите индекс и значение для вставки в первый массив: ";
    cin >> idx >> val;
    if (arr1.insertAt(idx, val))
        cout << "После вставки: " << arr1 << endl;
    else
        cout << "Неверный индекс!\n";

    // Удаление элемента по индексу
    cout << "\nВведите индекс для удаления из первого массива: ";
    cin >> idx;
    if (arr1.removeAt(idx))
        cout << "После удаления: " << arr1 << endl;
    else
        cout << "Неверный индекс!\n";

    // Удаление первого вхождения
    cout << "\nВведите значение для удаления (первое вхождение) из первого массива: ";
    cin >> val;
    if (arr1.removeValue(val))
        cout << "После удаления: " << arr1 << endl;
    else
        cout << "Элемент не найден!\n";

    // Удаление всех вхождений
    cout << "\nВведите значение для удаления всех вхождений из первого массива: ";
    cin >> val;
    arr1.removeAll(val);
    cout << "После удаления всех: " << arr1 << endl;

    // Сортировка по убыванию
    arr1.sort2();
    cout << "\nПосле сортировки по убыванию: " << arr1 << endl;

    // Сортировка по возрастанию
    arr1.sort();
    cout << "\nПосле сортировки по возрастанию: " << arr1 << endl;

    // Максимум и минимум
    if (arr1.getLength() > 0)
        cout << "Максимум: " << arr1.getMax() << ", Минимум: " << arr1.getMin() << endl;

    // Тестирование итераторов
    cout << "\nТестирование итераторов" << endl;
    cout << "Первый массив: " << arr1 << endl;

    // Использование begin() и end()
    cout << "Элементы через итераторы: ";
    for (auto it = arr1.begin(); it != arr1.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    // Вставка перед итератором
    cout << "\nВставка перед итератором:" << endl;
    cout << "Введите индекс элемента, перед которым вставить: ";
    cin >> idx;
    cout << "Введите значение для вставки: ";
    cin >> val;

    if (idx >= 0 && idx < arr1.getLength()) {
        auto it = arr1.begin() + idx;
        if (arr1.insertBefore(it, val)) {
            cout << "После вставки: " << arr1 << endl;
        }
        else {
            cout << "Ошибка вставки!" << endl;
        }
    }
    else {
        cout << "Неверный индекс!" << endl;
    }

    // Удаление по итератору
    cout << "\nУдаление по итератору:" << endl;
    cout << "Введите индекс элемента для удаления: ";
    cin >> idx;

    if (idx >= 0 && idx < arr1.getLength()) {
        auto it = arr1.begin() + idx;
        if (arr1.removeAtIterator(it)) {
            cout << "После удаления: " << arr1 << endl;
        }
        else {
            cout << "Ошибка удаления!" << endl;
        }
    }
    else {
        cout << "Неверный индекс!" << endl;
    }

    // Удаление диапазона
    cout << "\nУдаление диапазона по итераторам:" << endl;
    int startIdx, endIdx;
    cout << "Введите начальный индекс диапазона: ";
    cin >> startIdx;
    cout << "Введите конечный индекс диапазона: ";
    cin >> endIdx;

    if (startIdx >= 0 && endIdx <= arr1.getLength() && startIdx < endIdx) {
        auto first = arr1.begin() + startIdx;
        auto last = arr1.begin() + endIdx;
        if (arr1.removeRange(first, last)) {
            cout << "После удаления диапазона: " << arr1 << endl;
        }
        else {
            cout << "Ошибка удаления диапазона!" << endl;
        }
    }
    else {
        cout << "Неверные индексы диапазона!" << endl;
    }

    // Конкатенация массивов
    DynamicArray arr3 = arr1 + arr2;
    cout << "\nРезультат arr1 + arr2: " << arr3 << endl;

    arr1 += arr2;
    cout << "После arr1 += arr2: " << arr1 << endl;

    // Изменение элемента через []
    cout << "\nВведите индекс и новое значение для изменения элемента первого массива: ";
    cin >> idx >> val;
    if (idx >= 0 && idx < arr1.getLength())
    {
        arr1[idx] = val;
        cout << "После изменения: " << arr1 << endl;
    }
    else
    {
        cout << "Неверный индекс!\n";
    }

    // Функция swap
    cout << "\nФункция swap:" << endl;
    cout << "До swap:" << endl;
    cout << "arr1: " << arr1 << endl;
    cout << "arr2: " << arr2 << endl;

    DynamicArray temp1 = arr1;
    DynamicArray temp2 = arr2;

    temp1.swap(temp2);

    cout << "После swap:" << endl;
    cout << "arr1 (temp1): " << temp1 << endl;
    cout << "arr2 (temp2): " << temp2 << endl;

    return 0;
}