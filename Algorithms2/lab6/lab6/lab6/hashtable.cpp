#include <iostream>
#include <cmath>
#include <algorithm>
#include <utility>

#include <QTableWidget>
#include <QString>

#include "hashtable.h"

uint QuadraticHashFunction::hash(const uint key, const uint tableSize) const
{
    if (tableSize == 0) return 0;

    const uint c = 1; // 26 % 5
    const uint d = 5; // 26 % 7
    uint h0 = key % tableSize;

    return (h0 + c + d) % tableSize;
}

uint GoldenRatioHashFunction::hash(const uint key, const uint tableSize) const
{
    if (tableSize == 0) return 0;

    const double a = -(1.0 - std::sqrt(5.0)) / 2.0;
    uint h0 = key % tableSize;

    return static_cast<uint>(std::floor(static_cast<double>(h0) * a * static_cast<double>(tableSize))) % tableSize;
}

HashTable::HashTable(const uint defaultTableSize)
    : buckets_(defaultTableSize, nullptr), keysCount_(0)
{
    currentHashFunction_ = new GoldenRatioHashFunction();
}

HashTable::HashTable(const HashTable& other)
    : buckets_(other.buckets_.size(), nullptr), keysCount_(0), currentHashFunction_(nullptr)
{
    copyFrom(other);
}

HashTable::~HashTable()
{
    clearNodes();
    delete currentHashFunction_;
}

HashTable& HashTable::operator=(const HashTable& other)
{
    if (this != &other)
    {
        HashTable temp(other);
        swap(temp);
    }
    return *this;
}

void HashTable::clearNodes()
{
    for (size_t i = 0; i < buckets_.size(); ++i)
    {
        HashNode* current = buckets_[i];
        while (current)
        {
            HashNode* next = current->next;
            delete current;
            current = next;
        }
        buckets_[i] = nullptr;
    }
    keysCount_ = 0;
}

void HashTable::copyFrom(const HashTable& other)
{
    clearNodes();

    buckets_.assign(other.buckets_.size(), nullptr);
    keysCount_ = other.keysCount_;

    delete currentHashFunction_;
    if (dynamic_cast<QuadraticHashFunction*>(other.currentHashFunction_))
    {
        currentHashFunction_ = new QuadraticHashFunction();
    }
    else
    {
        currentHashFunction_ = new GoldenRatioHashFunction();
    }

    for (size_t i = 0; i < other.buckets_.size(); ++i)
    {
        HashNode* src = other.buckets_[i];
        HashNode** dest = &buckets_[i];

        while (src)
        {
            *dest = new HashNode(src->key, src->value);
            dest = &((*dest)->next);
            src = src->next;
        }
    }
}

bool HashTable::insert(const uint key, const std::string& value)
{
    if (buckets_.empty()) return false;

    uint index = currentHashFunction_->hash(key, static_cast<uint>(buckets_.size()));

    HashNode* current = buckets_[index];
    while (current)
    {
        if (current->key == key)
        {
            current->value = value;
            return true;
        }
        current = current->next;
    }

    buckets_[index] = new HashNode(key, value, buckets_[index]);
    ++keysCount_;

    return true;
}

bool HashTable::remove(const uint key)
{
    if (buckets_.empty()) return false;

    uint index = currentHashFunction_->hash(key, static_cast<uint>(buckets_.size()));

    HashNode* current = buckets_[index];
    HashNode* prev = nullptr;

    while (current)
    {
        if (current->key == key)
        {
            if (prev)
            {
                prev->next = current->next;
            }
            else
            {
                buckets_[index] = current->next;
            }

            delete current;
            --keysCount_;
            return true;
        }
        prev = current;
        current = current->next;
    }

    return false;
}

bool HashTable::contains(const uint key) const
{
    if (buckets_.empty()) return false;

    uint index = currentHashFunction_->hash(key, static_cast<uint>(buckets_.size()));

    HashNode* current = buckets_[index];
    while (current)
    {
        if (current->key == key)
        {
            return true;
        }
        current = current->next;
    }

    return false;
}

std::string& HashTable::operator[](const uint key)
{
    uint index = currentHashFunction_->hash(key, static_cast<uint>(buckets_.size()));

    HashNode* current = buckets_[index];
    while (current)
    {
        if (current->key == key)
        {
            return current->value;
        }
        current = current->next;
    }

    buckets_[index] = new HashNode(key, std::string(), buckets_[index]);
    ++keysCount_;

    return buckets_[index]->value;
}

void HashTable::swap(HashTable& other) noexcept
{
    std::swap(buckets_, other.buckets_);
    std::swap(keysCount_, other.keysCount_);
    std::swap(currentHashFunction_, other.currentHashFunction_);
}

void HashTable::setHashFunction(IHashFunction* newHashFunc)
{
    if (!newHashFunc || newHashFunc == currentHashFunction_) return;

    std::vector<std::pair<uint, std::string>> allEntries;
    allEntries.reserve(keysCount_);

    for (HashNode* head : buckets_)
    {
        HashNode* current = head;
        while (current)
        {
            allEntries.emplace_back(current->key, current->value);
            current = current->next;
        }
    }

    delete currentHashFunction_;
    currentHashFunction_ = newHashFunc;

    clearNodes();

    for (const auto& entry : allEntries)
    {
        insert(entry.first, entry.second);
    }
}

void HashTable::resize(const uint newSize)
{
    if (newSize == 0) return;

    std::vector<HashNode*> oldBuckets = std::move(buckets_);
    buckets_.assign(newSize, nullptr);
    keysCount_ = 0;

    for (HashNode* head : oldBuckets)
    {
        HashNode* current = head;
        while (current)
        {
            insert(current->key, current->value);
            HashNode* toDelete = current;
            current = current->next;
            delete toDelete;
        }
    }
}

void HashTable::printTableToConsole() const
{
    for (size_t i = 0; i < buckets_.size(); ++i)
    {
        std::cout << "[" << i << "]: ";

        HashNode* current = buckets_[i];
        if (!current)
        {
            std::cout << "None";
        }
        else
        {
            while (current)
            {
                std::cout << "{" << current->key << ": \"" << current->value << "\"}";
                if (current->next)
                {
                    std::cout << " -> ";
                }
                current = current->next;
            }
        }
        std::cout << std::endl;
    }
}

void HashTable::showInGui(QTableWidget* tableWidget) const
{
    if (!tableWidget) return;

    tableWidget->clearContents();
    tableWidget->setRowCount(0);

    size_t tableSize = buckets_.size();

    for (size_t i = 0; i < tableSize; ++i)
    {
        int row = static_cast<int>(i);
        tableWidget->insertRow(row);

        tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(i)));

        QString chainStr;
        HashNode* current = buckets_[i];

        if (current)
        {
            while (current)
            {
                chainStr += QString("%1 - %2")
                    .arg(current->key)
                    .arg(QString::fromStdString(current->value));
                if (current->next)
                {
                    chainStr += " -> ";
                }
                current = current->next;
            }
        }

        tableWidget->setItem(row, 1, new QTableWidgetItem(chainStr));
    }
}

uint HashTable::getKeysCount() const
{
    return keysCount_;
}

uint HashTable::getTableSize() const
{
    return static_cast<uint>(buckets_.size());
}