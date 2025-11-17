#pragma once

#include <assert.h>
#include <iostream>

class DynamicArray
{
public:
    // Конструктор по умолчанию ( создаёт пустой массив (нулевой длины, без выделенной памяти ) )
    DynamicArray();

    // Конструктор из обычного массива
    DynamicArray(const int* data, int initialLength);

    // Конструктор копирования ( создаёт новый объект как копию уже существующего массива )
    DynamicArray(const DynamicArray&);

    // Конструктор перемещения
    DynamicArray(DynamicArray&& other);

    // Деструктор ( освобождает динамически выделенную память при уничтожении объекта )
    ~DynamicArray();

    // Получение размера (количества хранимых элементов в настоящий момент)
    int getLength() const { return arrayLength_; }

    // Обмен содержимого с другим массивом (swap)
    void swap(DynamicArray& other);

    // Поиск элемента(возвращает индекс первого совпавшего элемента, либо - 1, если совпадений нет)
    int find(const int value) const;

    // Ввод/вывод в консоль (потоковый)
    friend std::ostream& operator<<(std::ostream& os, const DynamicArray& arr);
    friend std::istream& operator>>(std::istream& is, DynamicArray& arr);

    // Сортировка элементов (пузырьком)
    void sort();

    // Сортировка элементов по убыванию (пузырьком)
    void sort2();

    //  Вставка элемента по индексу. Если индекс некорректный, вернуть false
    bool insertAt(const int, const int);

    // Удаление элемента по индексу. Если индекс некорректный, вернуть false
    bool removeAt(const int index);

    // Удаление элемента по значению (первое вхождение). Если элемент отсутствует в массиве, вернуть false
    bool removeValue(const int value);

    // Оператор получение ссылки на элемент по индексу []
    int& operator[](const int index)
    {
        assert((index >= 0 && index < arrayLength_) && "Index is out of range.");
        return arrayData_[index];
    }

    // Константная версия оператора [] ( для доступа к элементам только на чтение )
    const int& operator[](const int index) const
    {
        assert((index >= 0 && index < arrayLength_) && "Index is out of range.");
        return arrayData_[index];
    }

    // Оператор присваивания копированием (=)
    DynamicArray& operator=(const DynamicArray&);

    // Оператор присваивания перемещением (=)
    DynamicArray& operator=(DynamicArray&& other);

    // Итераторы
    int* begin() { return arrayData_; }
    const int* begin() const { return arrayData_; }
    int* end() { return arrayData_ + arrayLength_; }
    const int* end() const { return arrayData_ + arrayLength_; }

    // Добавление элемента в конец массива
    void add(const int value);

    // Удаление всех элементов с заданным значением
    void removeAll(const int value);

    // Поиск максимального элемента
    int getMax() const;

    // Поиск минимального элемента
    int getMin() const;

    // Перегрузка сложение (конкатенация) с другим массивом (+ и +=)
    DynamicArray operator+(const DynamicArray& other) const;
    DynamicArray& operator+=(const DynamicArray& other);

    // Перегрузка сравнение (== и !=)
    bool operator==(const DynamicArray& other) const;
    bool operator!=(const DynamicArray& other) const;

    // Вставка перед указанным итератором
    bool insertBefore(int* iterator, const int value);

    // Удаление элемента по итератору
    bool removeAtIterator(int* iterator);

    // Удаление диапазона элементов
    bool removeRange(int* first, int* last);

private:
    int* arrayData_;
    int arrayLength_;
};