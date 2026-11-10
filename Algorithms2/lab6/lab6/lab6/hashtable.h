#pragma once

#include <string>
#include <vector>

using uint = unsigned int;

class QTableWidget;

// Базовый интерфейс хеш-функции
class IHashFunction
{
public:

    virtual ~IHashFunction() = default;
    virtual uint hash(const uint key, const uint tableSize) const = 0;
};

// Функция 1: c = 26 % 5 = 1, d = 26 % 7 = 5 -> h1(K) = (h0 + 6) mod N
class QuadraticHashFunction : public IHashFunction
{
public:

    virtual uint hash(const uint key, const uint tableSize) const override;
};

// Функция 2: h1(K) = [h0 * a * N] mod N, a = -(1 - sqrt(5)) / 2
class GoldenRatioHashFunction : public IHashFunction
{
public:

    virtual uint hash(const uint key, const uint tableSize) const override;
};

// Хеш-таблица (метод внешних цепочек)
class HashTable
{
public:

    struct HashNode
    {
        uint key;
        std::string value;
        HashNode* next = nullptr;

        HashNode(const uint k, const std::string& v, HashNode* n = nullptr)
            : key(k), value(v), next(n) {
        }
    };

    HashTable(const uint defaultTableSize = 25);
    HashTable(const HashTable& other);
    ~HashTable();

    HashTable& operator=(const HashTable& other);

    bool insert(const uint key, const std::string& value);
    bool remove(const uint key);
    bool contains(const uint key) const;
    void swap(HashTable& other) noexcept;
    void printTableToConsole() const;
    void showInGui(QTableWidget* tableWidget) const;
    void setHashFunction(IHashFunction* newHashFunc);
    void resize(const uint newSize);

    std::string& operator[](const uint key);

    uint getKeysCount() const;
    uint getTableSize() const;

private:

    void clearNodes();
    void copyFrom(const HashTable& other);

    std::vector<HashNode*> buckets_;
    uint keysCount_ = 0;

    IHashFunction* currentHashFunction_ = nullptr;
};