#include <iostream>
#include <string>
#include "dynamicarray.h"
#include <Windows.h>

using namespace std;

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // Конструктор из обычного массива
    int testArray[] = { 1, 2, 3, 4, 5 };
    int testLength = std::size(testArray);

    DynamicArray<int> testArr(testArray, testLength);
    cout << "Создан из обычного массива: " << testArr << endl;
    cout << "Длина: " << testArr.getLength() << endl << endl;

    // Конструктор копирования
    DynamicArray<int> arrCopy(testArr);
    cout << "Скопированный массив: " << arrCopy << endl;

    // Оператор присваивания
    DynamicArray<int> arrAssigned;
    arrAssigned = testArr;
    cout << "Присвоенный массив: " << arrAssigned << endl;

    // Ввод первого массива int
    DynamicArray<int> arr1;
    cin >> arr1;
    cout << "Первый массив: " << arr1 << "\n";

    // Ввод второго массива int
    DynamicArray<int> arr2;
    cin >> arr2;
    cout << "Второй массив: " << arr2 << "\n";

    // Ввод и вывод третьего массива unsigned int
    DynamicArray<unsigned int> arr4;
    cin >> arr4;
    cout << "Третий массив: " << arr4 << "\n";

    // Вставка
    int index, value;
    cout << "\nВведите индекс и значение для вставки в первый массив: ";
    cin >> index >> value;
    if (arr1.insertAt(index, value))
    {
        cout << "После вставки: [ ";
        for (int i = 0; i < arr1.getLength(); i++) cout << arr1[i] << (i < arr1.getLength() - 1 ? ", " : " ");
        cout << "]\n";
    }
    else cout << "Неверный индекс!\n";

    // Функция find
    int searchValue;
    cout << "Введите значение для поиска в первом массиве: ";
    cin >> searchValue;
    int foundIndex = arr1.find(searchValue);
    if (foundIndex != -1) {
        cout << "Элемент " << searchValue << " найден по индексу: " << foundIndex << endl;
    }
    else {
        cout << "Элемент " << searchValue << " не найден" << endl;
    }

    // Удаление по индексу
    cout << "\nВведите индекс для удаления из первого массива: ";
    cin >> index;
    if (arr1.removeAt(index))
    {
        cout << "После удаления: [ ";
        for (int i = 0; i < arr1.getLength(); i++) cout << arr1[i] << (i < arr1.getLength() - 1 ? ", " : " ");
        cout << "]\n";
    }
    else cout << "Неверный индекс!\n";

    // Удаление по значению (первое вхождение)
    cout << "\nВведите значение для удаления (первое вхождение) из первого массива: ";
    cin >> value;
    if (arr1.removeValue(value))
    {
        cout << "После удаления: [ ";
        for (int i = 0; i < arr1.getLength(); i++) cout << arr1[i] << (i < arr1.getLength() - 1 ? ", " : " ");
        cout << "]\n";
    }
    else cout << "Элемент не найден!\n";

    // Удаление всех вхождений
    cout << "\nВведите значение для удаления всех вхождений из первого массива: ";
    cin >> value;
    arr1.removeAll(value);
    cout << "После удаления всех: [ ";
    for (int i = 0; i < arr1.getLength(); i++) cout << arr1[i] << (i < arr1.getLength() - 1 ? ", " : " ");
    cout << "]\n";

    DynamicArray<int> arr1_copy = arr1;

    // Сортировка по убыванию
    arr1.sort2();
    cout << "\nПосле сортировки по убыванию: [ ";
    for (int i = 0; i < arr1.getLength(); i++) cout << arr1[i] << (i < arr1.getLength() - 1 ? ", " : " ");
    cout << "]\n";

    arr1 = arr1_copy;

    // Сортировка по возрастанию
    arr1.sort();
    cout << "\nПосле сортировки: [ ";
    for (int i = 0; i < arr1.getLength(); i++) cout << arr1[i] << (i < arr1.getLength() - 1 ? ", " : " ");
    cout << "]\n";
    
    // Максимум/Минимум
    cout << "Максимум: " << arr1.getMax() << ", Минимум: " << arr1.getMin() << "\n";

    // Тестирование итераторов
    cout << "\nТестирование итераторов" << endl;

    // Ввод тестового массива для итераторов
    DynamicArray<int> iterArr;
    cout << "Введите тестовый массив для работы с итераторами:\n";
    cin >> iterArr;
    cout << "Тестовый массив для итераторов: " << iterArr << endl;

    // Использование begin() и end()
    cout << "Элементы через итераторы: ";
    for (auto it = iterArr.begin(); it != iterArr.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    // Вставка перед итератором
    cout << "\nВставка перед итератором" << endl;
    cout << "Введите индекс элемента, перед которым вставить: ";
    cin >> index;
    cout << "Введите значение для вставки: ";
    cin >> value;

    if (index >= 0 && index < iterArr.getLength()) {
        auto it = iterArr.begin() + index;
        if (iterArr.insertBefore(it, value)) {
            cout << "После вставки: [ ";
            for (int i = 0; i < iterArr.getLength(); i++) cout << iterArr[i] << (i < iterArr.getLength() - 1 ? ", " : " ");
            cout << "]\n";
        }
        else {
            cout << "Ошибка вставки!" << endl;
        }
    }
    else {
        cout << "Неверный индекс!" << endl;
    }

    // Удаление по итератору
    cout << "\nУдаление по итератору" << endl;
    cout << "Введите индекс элемента для удаления: ";
    cin >> index;

    if (index >= 0 && index < iterArr.getLength()) {
        auto it = iterArr.begin() + index;
        if (iterArr.removeAtIterator(it)) {
            cout << "После удаления: [ ";
            for (int i = 0; i < iterArr.getLength(); i++) cout << iterArr[i] << (i < iterArr.getLength() - 1 ? ", " : " ");
            cout << "]\n";
        }
        else {
            cout << "Ошибка удаления!" << endl;
        }
    }
    else {
        cout << "Неверный индекс!" << endl;
    }

   // Удаление диапазона
    cout << "\nУдаление диапазона" << endl;
    int startIdx, endIdx;
    cout << "Введите начальный индекс диапазона: ";
    cin >> startIdx;
    cout << "Введите конечный индекс диапазона: ";
    cin >> endIdx;

    if (startIdx >= 0 && endIdx <= iterArr.getLength() && startIdx < endIdx) {
        auto first = iterArr.begin() + startIdx;
        auto last = iterArr.begin() + endIdx;
        if (iterArr.removeRange(first, last)) {
            cout << "После удаления диапазона: [ ";
            for (int i = 0; i < iterArr.getLength(); i++) cout << iterArr[i] << (i < iterArr.getLength() - 1 ? ", " : " ");
            cout << "]\n";
        }
        else {
            cout << "Ошибка удаления диапазона!" << endl;
        }
    }
    else {
        cout << "Неверные индексы диапазона!" << endl;
    }


    // Изменение элемента
    cout << "\nВведите индекс и новое значение для изменения элемента первого массива: ";
    cin >> index >> value;
    if (index >= 0 && index < arr1.getLength())
    {
        arr1[index] = value;
        cout << "После изменения: [ ";
        for (int i = 0; i < arr1.getLength(); i++) cout << arr1[i] << (i < arr1.getLength() - 1 ? ", " : " ");
        cout << "]\n";
    }
    else cout << "Неверный индекс!\n";



    // Функция swap
    DynamicArray<int> swap1 = arr1;
    DynamicArray<int> swap2 = arr2;

    cout << "До swap:" << endl;
    cout << "swap1: " << swap1 << endl;
    cout << "swap2: " << swap2 << endl;

    swap1.swap(swap2);

    cout << "После swap:" << endl;
    cout << "swap1: " << swap1 << endl;
    cout << "swap2: " << swap2 << endl;

    // Конкатенация массивов
    DynamicArray<int> arr3 = arr1 + arr2;
    cout << "\nРезультат arr1 + arr2: [ ";
    for (int i = 0; i < arr3.getLength(); i++) cout << arr3[i] << (i < arr3.getLength() - 1 ? ", " : " ");
    cout << "]\n";

    arr1 += arr2;
    cout << "После arr1 += arr2: [ ";
    for (int i = 0; i < arr1.getLength(); i++) cout << arr1[i] << (i < arr1.getLength() - 1 ? ", " : " ");
    cout << "]\n";

    // Сравнение массивов
    cout << "\nСравнение arr1 и arr3:\n";
    cout << "arr1 == arr3? " << (arr1 == arr3 ? "Да" : "Нет") << "\n";
    cout << "arr1 != arr2? " << (arr1 != arr2 ? "Да" : "Нет") << "\n";

    return 0;
}