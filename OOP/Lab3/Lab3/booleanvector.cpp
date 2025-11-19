#include <stdexcept>
#include <cstring>
#include <iostream>
#include "booleanvector.h"

BooleanVector::BooleanVector(const uint32_t numBits, const bool initialValue)
    : numBits_(numBits), numBytes_((numBits + 7) / 8)
{
    if (numBytes_ > 0) {
        vectorData_ = new uint8_t[numBytes_];
        uint8_t initialByteValue = initialValue ? 255 : 0;
        for (uint32_t i = 0; i < numBytes_; i++) {
            vectorData_[i] = initialByteValue;
        }
    }
    else {
        vectorData_ = nullptr;
    }
}

BooleanVector::BooleanVector(const char* str)
{
    if (str == nullptr) {
        numBits_ = 0;
        vectorData_ = nullptr;
        numBytes_ = 0;
        return;
    }

    // Определяем длину строки
    uint32_t length = 0;
    while (str[length] != '\0') length++;
    numBits_ = length;
    numBytes_ = (numBits_ + 7) / 8;

    if (numBytes_ > 0) {
        vectorData_ = new uint8_t[numBytes_];
        // Инициализируем нулями
        for (uint32_t i = 0; i < numBytes_; i++) {
            vectorData_[i] = 0;
        }

        // Заполняем вектор из строки
        for (uint32_t i = 0; i < numBits_; i++) {
            if (str[i] == '1') {
                uint32_t byteIndex = i / 8;
                uint32_t bitIndex = i % 8;
                vectorData_[byteIndex] |= (1 << bitIndex);
            }
        }
    }
    else {
        vectorData_ = nullptr;
    }
}

BooleanVector::BooleanVector(const BooleanVector& other)
    : numBits_(other.numBits_), numBytes_(other.numBytes_)
{
    if (other.vectorData_ != nullptr && numBytes_ > 0) {
        vectorData_ = new uint8_t[numBytes_];
        std::memcpy(vectorData_, other.vectorData_, numBytes_);
    }
    else {
        vectorData_ = nullptr;
    }
}

BooleanVector::~BooleanVector()
{
    delete[] vectorData_;
    vectorData_ = nullptr;
    numBits_ = 0;
    numBytes_ = 0;
}

BooleanVector& BooleanVector::operator=(const BooleanVector& other)
{
    if (this != &other) {
        delete[] vectorData_;

        numBits_ = other.numBits_;
        numBytes_ = other.numBytes_;

        if (other.vectorData_ != nullptr && numBytes_ > 0) {
            vectorData_ = new uint8_t[numBytes_];
            std::memcpy(vectorData_, other.vectorData_, numBytes_);
        }
        else {
            vectorData_ = nullptr;
        }
    }
    return *this;
}

BooleanVector& BooleanVector::operator=(BooleanVector&& other)
{
    if (this != &other) {
        delete[] vectorData_;

        vectorData_ = other.vectorData_;
        numBits_ = other.numBits_;
        numBytes_ = other.numBytes_;

        other.vectorData_ = nullptr;
        other.numBits_ = 0;
        other.numBytes_ = 0;
    }
    return *this;
}

bool BooleanVector::operator[](const uint32_t index) const {
    if (index >= numBits_)
        throw std::runtime_error("Индекс выходит за границы.");

    uint32_t byteIndex = index / 8;
    uint32_t bitIndex = index % 8;

    return (vectorData_[byteIndex] >> bitIndex) & 1;
}

uint32_t BooleanVector::getWeight() const
{
    uint32_t weight = 0;
    for (uint32_t i = 0; i < numBits_; i++) {
        if ((*this)[i]) {
            weight++;
        }
    }
    return weight;
}

void BooleanVector::swap(BooleanVector& other)
{
    std::swap(vectorData_, other.vectorData_);
    std::swap(numBits_, other.numBits_);
    std::swap(numBytes_, other.numBytes_);
}

void BooleanVector::invert()
{
    for (uint32_t i = 0; i < numBytes_; i++) {
        vectorData_[i] = ~vectorData_[i];
    }

    // Корректируем последний байт, если нужно
    if (numBits_ % 8 != 0) {
        uint8_t mask = (1 << (numBits_ % 8)) - 1;
        vectorData_[numBytes_ - 1] &= mask;
    }
}

void BooleanVector::setBit(const uint32_t index, const bool value)
{
    if (index >= numBits_)
        throw std::runtime_error("Индекс выходит за границы.");

    uint32_t byteIndex = index / 8;
    uint32_t bitIndex = index % 8;

    if (value) {
        vectorData_[byteIndex] |= (1 << bitIndex);
    }
    else {
        vectorData_[byteIndex] &= ~(1 << bitIndex);
    }
}

void BooleanVector::invertBit(const uint32_t index)
{
    if (index >= numBits_)
        throw std::runtime_error("Индекс выходит за границы");

    uint32_t byteIndex = index / 8;
    uint32_t bitIndex = index % 8;

    // Инвертируем бит с помощью XOR
    vectorData_[byteIndex] ^= (1 << bitIndex);
}

void BooleanVector::setBits(const uint32_t startIndex, const uint32_t count, const bool value)
{
    if (startIndex >= numBits_)
        throw std::runtime_error("Стартовый индекс выходит за границы.");

    if (count == 0)
        return;

    uint32_t endIndex = startIndex + count;
    if (endIndex > numBits_)
        throw std::runtime_error("Количество компонент выходит за границы вектора.");

    for (uint32_t i = startIndex; i < endIndex; i++) {
        uint32_t byteIndex = i / 8;
        uint32_t bitIndex = i % 8;

        if (value) {
            vectorData_[byteIndex] |= (1 << bitIndex);
        }
        else {
            vectorData_[byteIndex] &= ~(1 << bitIndex);
        }
    }
}

void BooleanVector::setAll(const bool value)
{
    if (numBytes_ == 0)
        return;

    uint8_t byteValue = value ? 255 : 0;
    for (uint32_t i = 0; i < numBytes_; i++) {
        vectorData_[i] = byteValue;
    }

    // Корректируем последний байт, если нужно
    if (numBits_ % 8 != 0) {
        uint8_t mask = (1 << (numBits_ % 8)) - 1;
        if (value) {
            vectorData_[numBytes_ - 1] |= mask;
        }
        else {
            vectorData_[numBytes_ - 1] &= ~mask;
        }
    }
}

BooleanVector BooleanVector::operator&(const BooleanVector& other) const
{
    if (numBits_ != other.numBits_) {
        throw std::runtime_error("Векторы имеют разную длину");
    }

    BooleanVector result(numBits_, false);
    for (uint32_t i = 0; i < numBytes_; i++) {
        result.vectorData_[i] = vectorData_[i] & other.vectorData_[i];
    }
    return result;
}

BooleanVector BooleanVector::operator|(const BooleanVector& other) const
{
    if (numBits_ != other.numBits_) {
        throw std::runtime_error("Векторы имеют разную длниу");
    }

    BooleanVector result(numBits_, false);
    for (uint32_t i = 0; i < numBytes_; i++) {
        result.vectorData_[i] = vectorData_[i] | other.vectorData_[i];
    }
    return result;
}

BooleanVector BooleanVector::operator^(const BooleanVector& other) const
{
    if (numBits_ != other.numBits_) {
        throw std::runtime_error("Векторы имеют разную длину");
    }

    BooleanVector result(numBits_, false);
    for (uint32_t i = 0; i < numBytes_; i++) {
        result.vectorData_[i] = vectorData_[i] ^ other.vectorData_[i];
    }
    return result;
}

BooleanVector BooleanVector::operator~() const
{
    BooleanVector result(*this);
    result.invert();
    return result;
}

//Побитовые сдвиги
BooleanVector BooleanVector::operator<<(const uint32_t shift) const
{
    if (shift == 0) {
        return BooleanVector(*this);  // Возвращаем копию
    }

    if (shift >= numBits_) {
        // Если сдвиг больше или равен длине - возвращаем нулевой вектор
        return BooleanVector(numBits_, false);
    }

    BooleanVector result(numBits_, false);

    // Выполняем сдвиг
    for (uint32_t i = 0; i < numBits_ - shift; i++) {
        if ((*this)[i + shift]) {
            result.setBit(i, 1);
        }
    }

    return result;
}

BooleanVector BooleanVector::operator>>(const uint32_t shift) const
{
    if (shift == 0) {
        return BooleanVector(*this);  // Возвращаем копию
    }

    if (shift >= numBits_) {
        // Если сдвиг больше или равен длине - возвращаем нулевой вектор
        return BooleanVector(numBits_, false);
    }

    BooleanVector result(numBits_, false);

    // Выполняем сдвиг
    for (uint32_t i = shift; i < numBits_; i++) {
        if ((*this)[i - shift]) {
            result.setBit(i, 1);
        }
    }

    return result;
}

// Циклические сдвиги
BooleanVector BooleanVector::cyclicShiftLeft(const uint32_t shift) const
{
    if (numBits_ == 0) {
        return BooleanVector(*this);
    }

    uint32_t actualShift = shift % numBits_; // Нормализуем сдвиг
    if (actualShift == 0) {
        return BooleanVector(*this); // Возвращаем копию
    }

    BooleanVector result(numBits_, false);

    // Выполняем циклический сдвиг влево
    for (uint32_t i = 0; i < numBits_; i++) {
        uint32_t newIndex = (i + actualShift) % numBits_;
        if ((*this)[i]) {
            result.setBit(newIndex, 1);
        }
    }

    return result;
}

BooleanVector BooleanVector::cyclicShiftRight(const uint32_t shift) const
{
    if (numBits_ == 0) {
        return BooleanVector(*this);
    }

    uint32_t actualShift = shift % numBits_; // Нормализуем сдвиг
    if (actualShift == 0) {
        return BooleanVector(*this); // Возвращаем копию
    }

    BooleanVector result(numBits_, false);

    // Выполняем циклический сдвиг вправо
    for (uint32_t i = 0; i < numBits_; i++) {
        uint32_t newIndex = (i + numBits_ - actualShift) % numBits_;
        if ((*this)[i]) {
            result.setBit(newIndex, 1);
        }
    }

    return result;
}

// ввод/вывод
std::ostream& operator<<(std::ostream& os, const BooleanVector& bv)
{
    for (uint32_t i = 0; i < bv.numBits_; i++) {
        os << (bv[i] ? '1' : '0');
    }
    return os;
}

std::istream& operator>>(std::istream& is, BooleanVector& bv)
{
    std::string input;
    is >> input;

    // Очищаем текущие данные
    delete[] bv.vectorData_;

    bv.numBits_ = input.length();
    bv.numBytes_ = (bv.numBits_ + 7) / 8;

    if (bv.numBytes_ > 0) {
        bv.vectorData_ = new uint8_t[bv.numBytes_];
        // Инициализируем нулями
        for (uint32_t i = 0; i < bv.numBytes_; i++) {
            bv.vectorData_[i] = 0;
        }

        for (uint32_t i = 0; i < bv.numBits_; i++) {
            if (input[i] == '1') {
                bv.setBit(i, 1);
            }
            else if (input[i] == '0') {
                bv.setBit(i, 0);
            }
            else {
                throw std::runtime_error("Введён неверный символ (не 0 или 1)");
            }
        }
    }
    else {
        bv.vectorData_ = nullptr;
    }

    return is;
}