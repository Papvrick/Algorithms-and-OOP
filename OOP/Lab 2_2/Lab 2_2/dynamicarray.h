#pragma once

#include <assert.h>
#include <iostream>

// Шаблонный класс динамического массива
template<typename ItemType>
class DynamicArray
{
public:

    DynamicArray(); // Конструктор по умолчанию
    DynamicArray(const ItemType* arr, int initialLength); // Конструктор из обычного массива
    DynamicArray(const DynamicArray&); // Конструктор копирования
    DynamicArray(DynamicArray&&); // Конструктор перемещения
    ~DynamicArray(); // Деструктор

    DynamicArray& operator=(const DynamicArray&); // Присваивание копированием
    DynamicArray& operator=(DynamicArray&&); // Присваивание перемещением
    
    // Итераторы
    ItemType* begin() { return arrayData_; }                                    
    const ItemType* begin() const { return arrayData_; }                        
    ItemType* end() { return arrayData_ + arrayLength_; }                       
    const ItemType* end() const { return arrayData_ + arrayLength_; }           

    // Две версии индексирования
    ItemType& operator[](const int index)
    {
        assert((index >= 0 && index < arrayLength_) && "Index is out of range.");
        return arrayData_[index];
    }

    const ItemType& operator[](const int index) const
    {
        assert((index >= 0 && index < arrayLength_) && "Index is out of range.");
        return arrayData_[index];
    }

    // Вставка по индексу
    bool insertAt(const int, const ItemType&);

    // Добавление в конец
    void add(const ItemType&);

    // Получение размера
    int getLength() const { return arrayLength_; }

    // Получение указателя на данные
    ItemType* getData() const { return arrayData_; }

    // Вставка перед итератором
    bool insertBefore(ItemType* iterator, const ItemType& value);

    // Удаление по итератору
    bool removeAtIterator(ItemType* iterator);

    // Удаление диапазаона по итераторам
    bool removeRange(ItemType* first, ItemType* last);

    // Удаление элемента по индексу. Если индекс некорректный, вернуть false
    bool removeAt(const int index);

    // Поиск элемента(возвращает индекс первого совпавшего элемента, либо -1, если совпадений нет)
    int find(const ItemType value) const;

    // Удаление элемента по значению (первое вхождение). Если элемент отсутствует в массиве, вернуть false
    bool removeValue(const ItemType value);

    // Удаление всех элементов с заданным значением
    void removeAll(const ItemType value);

    // Сортировка элементов массива по возрастанию (Пузырьком)
    void sort();
    
    // Сортировка элементов массива по убыванию (Пузырьком)
    void sort2();

    // Поиск максимального элемента
    ItemType getMax() const;

    // Поиск минимального элемента
    ItemType getMin() const;

    // Перегрузка сложение (конкатенация) с другим массивом (+)
    DynamicArray operator+(const DynamicArray& other) const;

    // Перегрузка сложение (конкатенация) с другим массивом (+=)
    DynamicArray& operator+=(const DynamicArray& other);

    // Перегрузка сравнение (==)
    bool operator==(const DynamicArray& other) const;

    // Перегрузка сравнение (!=)
    bool operator!=(const DynamicArray& other) const;

    // Обмен содержимого с другим массивом (swap)
    void swap(DynamicArray& other);

    // Потоковый вывод
    friend std::ostream& operator<<(std::ostream& os, const DynamicArray<ItemType>& arr) {
        os << "[ ";
        for (int i = 0; i < arr.arrayLength_; ++i) {
            os << arr.arrayData_[i];
            if (i < arr.arrayLength_ - 1)
                os << ", ";
        }
        os << " ]";
        return os;
    }
    // Потоковый ввод 
    friend std::istream& operator>>(std::istream& is, DynamicArray<ItemType>& arr) {
        int newLength;
        std::cout << "\nВведите количество элементов: ";
        is >> newLength;

        // Проверка на корректность длины
        if (newLength < 0) {
            newLength = 0;
        }

        // Освобождаем старую память
        delete[] arr.arrayData_;

        if (newLength > 0) {
            arr.arrayLength_ = newLength;
            arr.arrayData_ = new ItemType[newLength];

            std::cout << "Введите " << newLength << " элементов через пробел: ";
            for (int i = 0; i < newLength; ++i) {
                is >> arr.arrayData_[i];
            }
        }
        else {
            // Создаем пустой массив
            arr.arrayData_ = nullptr;
            arr.arrayLength_ = 0;
        }

        return is;
    }

private:

    ItemType* arrayData_;
    int arrayLength_;
};

// Конструктор из обычного массива 
template<typename ItemType>
DynamicArray<ItemType>::DynamicArray(const ItemType* arr, int initialLength)
{
    if (!arr || initialLength <= 0) {
        arrayData_ = nullptr;
        arrayLength_ = 0; // Создан пустой массив
        return;
    }

    arrayLength_ = initialLength;
    arrayData_ = new ItemType[arrayLength_];

    // Копируем данные из переданного массива
    for (int i = 0; i < arrayLength_; ++i) {
        arrayData_[i] = arr[i];
    }
}

// Конструктор по умолчанию
template<typename ItemType>
DynamicArray<ItemType>::DynamicArray() : arrayData_(nullptr), arrayLength_(0)
{}

// Конструктор копирования
template<typename ItemType>
DynamicArray<ItemType>::DynamicArray(const DynamicArray<ItemType>& otherArray)
{
    if (otherArray.arrayLength_ == 0)
    {
        arrayData_ = nullptr;
        arrayLength_ = 0;
        return;
    }

    arrayLength_ = otherArray.arrayLength_;
    arrayData_ = new ItemType[arrayLength_];

    for (int index = 0; index < arrayLength_; ++index)
    {
        arrayData_[index] = otherArray.arrayData_[index];
    }
}

// Конструктор перемещения
template<typename ItemType>
DynamicArray<ItemType>::DynamicArray(DynamicArray<ItemType>&& otherArray)
{
    arrayData_ = otherArray.arrayData_;
    arrayLength_ = otherArray.arrayLength_;
    otherArray.arrayData_ = nullptr;
    otherArray.arrayLength_ = 0;
}

// Деструктор
template<typename ItemType>
DynamicArray<ItemType>::~DynamicArray()
{
    delete[] arrayData_;
}

// Оператор присваивания копированием
template<typename ItemType>
DynamicArray<ItemType>& DynamicArray<ItemType>::operator=(const DynamicArray<ItemType>& otherArray)
{
    if (this != &otherArray) // Защита от самоприсваивания
    {
        delete[] arrayData_; // Освобождение старой памяти

        if (otherArray.arrayLength_ == 0)
        {
            arrayData_ = nullptr;
            arrayLength_ = 0;
        }
        else
        {
            arrayLength_ = otherArray.arrayLength_;
            arrayData_ = new ItemType[arrayLength_];

            for (int index = 0; index < arrayLength_; ++index) // Копируем элементы
            {
                arrayData_[index] = otherArray.arrayData_[index];
            }
        }
    }

    return *this;
}

// Оператор присваивания перемещением
template<typename ItemType>
DynamicArray<ItemType>& DynamicArray<ItemType>::operator=(DynamicArray<ItemType>&& otherArray)
{
    if (this != &otherArray)
    {
        delete[] arrayData_;

        arrayData_ = otherArray.arrayData_;
        arrayLength_ = otherArray.arrayLength_;
        otherArray.arrayData_ = nullptr;
        otherArray.arrayLength_ = 0;
    }

    return *this;
}

// Вставка элемента по индексу
template<typename ItemType>
bool DynamicArray<ItemType>::insertAt(const int index, const ItemType& value)
{
    if (index == 0 && arrayLength_ == 0) // Вставка в пустой массив
    {
        arrayData_ = new ItemType[1];

        arrayData_[0] = value;

        ++arrayLength_;

        return true;
    }

    if (index < 0 || index > arrayLength_) return false; // Проверка индекса

    ItemType* tempArrayData = new ItemType[arrayLength_ + 1];

    for (int curIdx = 0; curIdx < index; ++curIdx) // Копирование элемента до позиции вставки
    {
        tempArrayData[curIdx] = arrayData_[curIdx];
    }

    for (int curIdx = index; curIdx < arrayLength_; ++curIdx) // Копирование элемента после позиции вставки
    {
        tempArrayData[curIdx + 1] = arrayData_[curIdx];
    }

    tempArrayData[index] = value;

    ++arrayLength_;

    delete[] arrayData_;

    arrayData_ = tempArrayData;

    return true;
}

// Добавление элемента в конец
template<typename ItemType>
void DynamicArray<ItemType>::add(const ItemType& value)
{
    ItemType* tempArrayData = new ItemType[arrayLength_ + 1];

    for (int index = 0; index < arrayLength_; ++index)
    {
        tempArrayData[index] = arrayData_[index];
    }

    tempArrayData[arrayLength_] = value;

    ++arrayLength_;

    delete[] arrayData_;
    arrayData_ = tempArrayData;
}

// Вставка элемента перед указанным итератором
template<typename ItemType>
bool DynamicArray<ItemType>::insertBefore(ItemType* iterator, const ItemType& value)
{
    if (iterator < arrayData_ || iterator > arrayData_ + arrayLength_)
        return false;

    int index = iterator - arrayData_;
    return insertAt(index, value);  
}

// Поиск элемента
template<typename ItemType>
int DynamicArray<ItemType>::find(const ItemType value) const
{
    for (int i = 0; i < arrayLength_; ++i)
        if (arrayData_[i] == value)
            return i;
    return -1;
}

// Удаление элемента по индексу
template<typename ItemType>
bool DynamicArray<ItemType>::removeAt(const int index)
{
    if (index < 0 || index >= arrayLength_) return false;

    ItemType* tempArrayData = (arrayLength_ > 1) ? new ItemType[arrayLength_ - 1] : nullptr;

    for (int i = 0, j = 0; i < arrayLength_; ++i)
        if (i != index)
            tempArrayData[j++] = arrayData_[i];

    delete[] arrayData_;
    arrayData_ = tempArrayData;
    --arrayLength_;
    return true;
}

// Удаление элемента по итератору
template<typename ItemType>
bool DynamicArray<ItemType>::removeAtIterator(ItemType* iterator)
{
    if (iterator < arrayData_ || iterator >= arrayData_ + arrayLength_)
        return false;

    int index = iterator - arrayData_;
    return removeAt(index);  
}

// Удаление первого вхождения по значению
template<typename ItemType>
bool DynamicArray<ItemType>::removeValue(const ItemType value)
{
    int index = find(value);
    if (index == -1) return false;
    return removeAt(index);
}

// Удаление всех вхождений по значению
template<typename ItemType>
void DynamicArray<ItemType>::removeAll(const ItemType value)
{
    if (arrayLength_ == 0) return;

    int countToKeep = 0;
    for (int i = 0; i < arrayLength_; ++i)
        if (arrayData_[i] != value)
            ++countToKeep;

    if (countToKeep == arrayLength_) return;

    ItemType* tempArrayData = (countToKeep > 0) ? new ItemType[countToKeep] : nullptr;

    int j = 0;
    for (int i = 0; i < arrayLength_; ++i)
        if (arrayData_[i] != value)
            tempArrayData[j++] = arrayData_[i];

    delete[] arrayData_;
    arrayData_ = tempArrayData;
    arrayLength_ = countToKeep;
}

// Удаление диапазона элементов по итераторам
template<typename ItemType>
bool DynamicArray<ItemType>::removeRange(ItemType* first, ItemType* last)
{
    if (first < arrayData_ || last > arrayData_ + arrayLength_ || first >= last)
        return false;

    int startIndex = first - arrayData_;
    int endIndex = last - arrayData_;
    int rangeLength = endIndex - startIndex;

    if (rangeLength <= 0) return false;

    ItemType* tempArrayData = (arrayLength_ > rangeLength) ? new ItemType[arrayLength_ - rangeLength] : nullptr;

    for (int i = 0; i < startIndex; ++i)
        tempArrayData[i] = arrayData_[i];

    for (int i = endIndex, j = startIndex; i < arrayLength_; ++i, ++j)
        tempArrayData[j] = arrayData_[i];

    delete[] arrayData_;
    arrayData_ = tempArrayData;
    arrayLength_ -= rangeLength;

    return true;
}

// Сортировка по возрастанию (пузырьком)
template<typename ItemType>
void DynamicArray<ItemType>::sort()
{
    for (int i = 0; i < arrayLength_ - 1; ++i)
        for (int j = 0; j < arrayLength_ - i - 1; ++j)
            if (arrayData_[j] > arrayData_[j + 1])
            {
                ItemType temp = arrayData_[j];
                arrayData_[j] = arrayData_[j + 1];
                arrayData_[j + 1] = temp;
            }
}

// Сортировка по убыванию (пузырьком)
template<typename ItemType>
void DynamicArray<ItemType>::sort2()
{
    for (int i = 0; i < arrayLength_ - 1; ++i)
        for (int j = 0; j < arrayLength_ - i - 1; ++j)
            if (arrayData_[j] < arrayData_[j + 1])
            {
                ItemType temp = arrayData_[j];
                arrayData_[j] = arrayData_[j + 1];
                arrayData_[j + 1] = temp;
            }
}

// Поиск максимального элемента
template<typename ItemType>
ItemType DynamicArray<ItemType>::getMax() const
{
    assert(arrayLength_ > 0 && "Array is empty.");
    ItemType maxVal = arrayData_[0];
    for (int i = 1; i < arrayLength_; ++i)
        if (arrayData_[i] > maxVal)
            maxVal = arrayData_[i];
    return maxVal;
}

// Поиск минимального элемента
template<typename ItemType>
ItemType DynamicArray<ItemType>::getMin() const
{
    assert(arrayLength_ > 0 && "Array is empty.");
    ItemType minVal = arrayData_[0];
    for (int i = 1; i < arrayLength_; ++i)
        if (arrayData_[i] < minVal)
            minVal = arrayData_[i];
    return minVal;
}

// Конкатенация массивов (+)
template<typename ItemType>
DynamicArray<ItemType> DynamicArray<ItemType>::operator+(const DynamicArray& other) const
{
    DynamicArray result;
    result.arrayLength_ = arrayLength_ + other.arrayLength_;
    result.arrayData_ = new ItemType[result.arrayLength_];

    for (int i = 0; i < arrayLength_; ++i)
        result.arrayData_[i] = arrayData_[i];

    for (int j = 0; j < other.arrayLength_; ++j)
        result.arrayData_[arrayLength_ + j] = other.arrayData_[j];

    return result;
}

// Конкатенация с присваиванием (+=)
template<typename ItemType>
DynamicArray<ItemType>& DynamicArray<ItemType>::operator+=(const DynamicArray& other)
{
    ItemType* tempArrayData = new ItemType[arrayLength_ + other.arrayLength_];

    for (int i = 0; i < arrayLength_; ++i)
        tempArrayData[i] = arrayData_[i];

    for (int j = 0; j < other.arrayLength_; ++j)
        tempArrayData[arrayLength_ + j] = other.arrayData_[j];

    delete[] arrayData_;
    arrayData_ = tempArrayData;
    arrayLength_ += other.arrayLength_;

    return *this;
}

// Опреатор сравнения на равенство
template<typename ItemType>
bool DynamicArray<ItemType>::operator==(const DynamicArray& other) const
{
    if (arrayLength_ != other.arrayLength_) return false;
    for (int i = 0; i < arrayLength_; ++i)
        if (arrayData_[i] != other.arrayData_[i])
            return false;
    return true;
}

// Оператор сравнения на неравенство
template<typename ItemType>
bool DynamicArray<ItemType>::operator!=(const DynamicArray& other) const
{
    return !(*this == other);
}

// Обмен содержимого двух массивов
template<typename ItemType>
void DynamicArray<ItemType>::swap(DynamicArray& other)
{
    ItemType* tempData = arrayData_;
    arrayData_ = other.arrayData_;
    other.arrayData_ = tempData;

    int tempLength = arrayLength_;
    arrayLength_ = other.arrayLength_;
    other.arrayLength_ = tempLength;
}