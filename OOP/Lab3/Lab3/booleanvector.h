#pragma once

#include <stdint.h>
#include <iostream>

class BooleanVector
{
public:
    BooleanVector() = default; //Конструктор по умолчанию
    BooleanVector(const uint32_t, const bool); //Конструктор с параметрами
    BooleanVector(const char*); //конструктор из массива const char*
    ~BooleanVector(); //Декструктор

    BooleanVector(const BooleanVector&); //Конструктор копирования

    BooleanVector& operator=(const BooleanVector&); //Операторы присваивания
    BooleanVector& operator=(BooleanVector&&);

    uint32_t getLength() const { return numBits_; } //Длина вектора
    uint32_t getWeight() const; //Вес вектора
    void swap(BooleanVector& other); // обмен с другим вектором
    void invert(); //инверсия всех компонент вектора
    void setBit(const uint32_t index, const bool value); //установка в 0/1 i-ой компоненты
    void invertBit(const uint32_t index);  // Инверсия i-ой компоненты

    // Побитовые операции
    bool operator[](const uint32_t) const;
    BooleanVector operator&(const BooleanVector& other) const; //Побитовое умножение
    BooleanVector operator|(const BooleanVector& other) const; //Побитовое сложение
    BooleanVector operator^(const BooleanVector& other) const; //Побитовое исключающее ИЛИ
    BooleanVector operator~() const; //Побитовая инверсия

    // Побитовые сдвиги
    BooleanVector operator<<(const uint32_t shift) const; //Сдвиг влево
    BooleanVector operator>>(const uint32_t shift) const; //Сдвиг вправо

    // Циклические сдвиги
    BooleanVector cyclicShiftLeft(const uint32_t shift) const;  // Циклический сдвиг влево
    BooleanVector cyclicShiftRight(const uint32_t shift) const; // Циклический сдвиг вправо

    // Потоковый ввод/вывод
    friend std::ostream& operator<<(std::ostream& os, const BooleanVector& bv);
    friend std::istream& operator>>(std::istream& is, BooleanVector& bv);

    // Установка в 0/1 k компонент, начиная с i-ой
    void setBits(const uint32_t startIndex, const uint32_t count, const bool value);

    // Установка в 0/1 всех компонент вектора
    void setAll(const bool value);

private:
    uint8_t* vectorData_ = nullptr;
    uint32_t numBits_ = 0;
    uint32_t numBytes_ = 0;
};