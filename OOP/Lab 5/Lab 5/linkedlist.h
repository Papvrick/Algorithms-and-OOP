#pragma once

#include <stdint.h>
#include <iostream>
#include <stdexcept>

template<typename ItemType>
class LinkedList
{
private:
    // Объявление класса узла списка 
    class ListNode;

    // Объявление классов итераторов
    class Iterator;
    class ConstIterator;

public:
    // Конструктор по умолчанию
    LinkedList() = default;

    // Конструктор из обычного массива
    LinkedList(const ItemType* array, uint32_t size);

    // Конструктор копирования
    LinkedList(const LinkedList<ItemType>& other);

    // Деструктор
    ~LinkedList();

    // Получение размера списка
    uint32_t getSize() const;

    // Обмен содержимого с другим списком (swap)
    void swap(LinkedList<ItemType>& other);

    // Проверка на пустоту списка
    bool isEmpty() const;

    // Очистка списка
    void clear();

    // Сортировка списка
    void sort();

    // Поиск элемента по ключу (возвращает указатель на элемент или nullptr, если элемента нет в списке)
    ItemType* find(const ItemType& key);
    const ItemType* find(const ItemType& key) const;

    // Поиск максимального элемента
    ItemType& getMax();
    const ItemType& getMax() const;

    // Поиск минимального элемента
    ItemType& getMin();
    const ItemType& getMin() const;

    // Добавление элемента в голову
    void addToHead(const ItemType& value);

    // Добавление элемента в хвост
    void addToTail(const ItemType& value);

    // Добавление элемента на позицию
    void addAtIndex(uint32_t index, const ItemType& value);

    // Добавление элемента после ключа (после первого вхождения)
    void addAfterKey(const ItemType& key, const ItemType& value);

    // Добавление элемента по итератору (после итератора)
    Iterator addAfterIterator(Iterator pos, const ItemType& value);

    // Удаление элемента из головы
    void removeFromHead();

    // Удаление элемента из хвоста
    void removeFromTail();

    // Удаление элемента из позиции
    void removeAtIndex(uint32_t index);

    // Удаление элемента по ключу (первое вхождение)
    void removeByKey(const ItemType& key);

    // Удаление элемента по итератору
    Iterator removeAtIterator(Iterator pos);

    // Удаление диапазона элементов с помощью итераторов [first, last)
    Iterator removeRange(Iterator first, Iterator last);

    // Получение итератора на начало списка
    Iterator begin();

    // Получение константного итератора на начало списка
    ConstIterator begin() const;

    // Получение итератора на конец списка (позиция после последнего элемента)
    Iterator end();

    // Получение константного итератора на конец списка
    ConstIterator end() const;

    // Присваивание (=)
    LinkedList<ItemType>& operator=(const LinkedList<ItemType>& other);

    // Получение ссылки на элемент по индексу ([ ])
    ItemType& operator[](uint32_t index);
    const ItemType& operator[](uint32_t index) const;

    // Сравнение (==, !=)
    bool operator==(const LinkedList<ItemType>& other) const;
    bool operator!=(const LinkedList<ItemType>& other) const;

    // Сложение (конкатенация) списков (+, +=)
    LinkedList<ItemType> operator+(const LinkedList<ItemType>& other) const;
    LinkedList<ItemType>& operator+=(const LinkedList<ItemType>& other);

    // Ввод/вывод в консоль (потоковый)
    friend std::ostream& operator<<(std::ostream& os, const LinkedList<ItemType>& list) {
        ListNode* current = list.headPtr_;
        while (current) {
            os << current->getValue() << " ";
            current = current->getLinkToNextNode();
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, LinkedList<ItemType>& list) {
        ItemType value;
        list.clear(); // Очищаем список перед вводом
        while (is >> value) {
            list.addToTail(value);
        }
        return is;
    }

private:
    ListNode* headPtr_ = nullptr;   // Указатель на голову списка
    ListNode* tailPtr_ = nullptr;   // Указатель на хвост списка
    uint32_t size_ = 0;             // Размер списка

    // Вспомогательный метод для получения узла по индексу
    ListNode* getNodeAtIndex(uint32_t index) const;

    // Вспомогательные методы для сортировки слиянием
    ListNode* mergeSort(ListNode* head);
    ListNode* split(ListNode* head);
    ListNode* merge(ListNode* left, ListNode* right);
};

// Класс узла списка 
template<typename ItemType>
class LinkedList<ItemType>::ListNode
{
public:
    // Конструктор узла списка
    ListNode(ItemType value = ItemType(), ListNode* next = nullptr, ListNode* prev = nullptr);

    // Получение значения узла
    ItemType& getValue();
    const ItemType& getValue() const;

    // Получение указателя на следующий узел
    ListNode* getLinkToNextNode();
    const ListNode* getLinkToNextNode() const;

    // Получение указателя на предыдущий узел
    ListNode* getLinkToPrevNode();
    const ListNode* getLinkToPrevNode() const;

    // Установка указателя на следующий узел
    void setLinkToNextNode(ListNode* next);

    // Установка указателя на предыдущий узел
    void setLinkToPrevNode(ListNode* prev);

private:
    ItemType value_;               // Значение, хранимое в узле
    ListNode* linkToNextNode_;     // Указатель на следующий узел
    ListNode* linkToPrevNode_;     // Указатель на предыдущий узел
};

// Класс итератора (обычный)
template<typename ItemType>
class LinkedList<ItemType>::Iterator
{
public:
    // Конструкторы
    Iterator() : node_(nullptr) {}
    explicit Iterator(ListNode* node) : node_(node) {}

    // Оператор разыменования
    ItemType& operator*() const { return node_->getValue(); }

    // Префиксный инкремент
    Iterator& operator++() {
        node_ = node_->getLinkToNextNode();
        return *this;
    }

    // Постфиксный инкремент
    Iterator operator++(int) {
        Iterator temp = *this;
        ++(*this);
        return temp;
    }

    // Операторы сравнения
    bool operator==(const Iterator& other) const { return node_ == other.node_; }
    bool operator!=(const Iterator& other) const { return node_ != other.node_; }

    // Получение узла (для внутреннего использования)
    ListNode* getNode() const { return node_; }

private:
    ListNode* node_;
};

// Класс итератора (константный)
template<typename ItemType>
class LinkedList<ItemType>::ConstIterator
{
public:
    // Конструкторы
    ConstIterator() : node_(nullptr) {}
    explicit ConstIterator(const ListNode* node) : node_(node) {}

    // Оператор разыменования
    const ItemType& operator*() const { return node_->getValue(); }

    // Префиксный инкремент
    ConstIterator& operator++() {
        node_ = node_->getLinkToNextNode();
        return *this;
    }

    // Постфиксный инкремент
    ConstIterator operator++(int) {
        ConstIterator temp = *this;
        ++(*this);
        return temp;
    }

    // Операторы сравнения
    bool operator==(const ConstIterator& other) const { return node_ == other.node_; }
    bool operator!=(const ConstIterator& other) const { return node_ != other.node_; }

    // Получение узла (для внутреннего использования)
    const ListNode* getNode() const { return node_; }

private:
    const ListNode* node_;
};

// Реализация методов класса LinkedList

// Конструктор из обычного массива
template<typename ItemType>
LinkedList<ItemType>::LinkedList(const ItemType* array, uint32_t size) {
    for (uint32_t i = 0; i < size; ++i) {
        addToTail(array[i]);
    }
}

// Конструктор копирования
template<typename ItemType>
LinkedList<ItemType>::LinkedList(const LinkedList<ItemType>& other) {
    ListNode* current = other.headPtr_;
    while (current) {
        addToTail(current->getValue());
        current = current->getLinkToNextNode();
    }
}

// Деструктор
template<typename ItemType>
LinkedList<ItemType>::~LinkedList() {
    clear();
}

// Получение размера списка
template<typename ItemType>
uint32_t LinkedList<ItemType>::getSize() const {
    return size_;
}

// Обмен содержимого с другим списком (swap)
template<typename ItemType>
void LinkedList<ItemType>::swap(LinkedList<ItemType>& other) {
    std::swap(headPtr_, other.headPtr_);
    std::swap(tailPtr_, other.tailPtr_);
    std::swap(size_, other.size_);
}

// Проверка на пустоту списка
template<typename ItemType>
bool LinkedList<ItemType>::isEmpty() const {
    return size_ == 0;
}

// Очистка списка
template<typename ItemType>
void LinkedList<ItemType>::clear() {
    while (!isEmpty()) {
        removeFromHead();
    }
}

// Сортировка списка
template<typename ItemType>
void LinkedList<ItemType>::sort() {
    if (size_ <= 1) return;

    headPtr_ = mergeSort(headPtr_);

    // Обновляем tailPtr_
    tailPtr_ = headPtr_;
    while (tailPtr_ && tailPtr_->getLinkToNextNode()) {
        tailPtr_ = tailPtr_->getLinkToNextNode();
    }
}

// Поиск элемента по ключу (возвращает указатель на элемент или nullptr, если элемента нет в списке)
template<typename ItemType>
ItemType* LinkedList<ItemType>::find(const ItemType& key) {
    ListNode* current = headPtr_;
    while (current) {
        if (current->getValue() == key) {
            return &(current->getValue());  // Возвращаем указатель на значение
        }
        current = current->getLinkToNextNode();
    }
    return nullptr;
}

template<typename ItemType>
const ItemType* LinkedList<ItemType>::find(const ItemType& key) const {
    const ListNode* current = headPtr_;
    while (current) {
        if (current->getValue() == key) {
            return &(current->getValue());  // Возвращаем константный указатель на значение
        }
        current = current->getLinkToNextNode();
    }
    return nullptr;
}

// Поиск максимального элемента
template<typename ItemType>
ItemType& LinkedList<ItemType>::getMax() {
    if (isEmpty()) {
        throw std::runtime_error("List is empty");
    }

    ListNode* current = headPtr_;
    ItemType* max = &(current->getValue());
    current = current->getLinkToNextNode();

    while (current) {
        if (current->getValue() > *max) {
            max = &(current->getValue());
        }
        current = current->getLinkToNextNode();
    }

    return *max;
}

template<typename ItemType>
const ItemType& LinkedList<ItemType>::getMax() const {
    if (isEmpty()) {
        throw std::runtime_error("List is empty");
    }

    const ListNode* current = headPtr_;
    const ListNode* maxNode = current;
    current = current->getLinkToNextNode();

    while (current) {
        if (current->getValue() > maxNode->getValue()) {
            maxNode = current;
        }
        current = current->getLinkToNextNode();
    }

    return maxNode->getValue();
}

// Поиск минимального элемента
template<typename ItemType>
ItemType& LinkedList<ItemType>::getMin() {
    if (isEmpty()) {
        throw std::runtime_error("List is empty");
    }

    ListNode* current = headPtr_;
    ItemType* min = &(current->getValue());
    current = current->getLinkToNextNode();

    while (current) {
        if (current->getValue() < *min) {
            min = &(current->getValue());
        }
        current = current->getLinkToNextNode();
    }

    return *min;
}

template<typename ItemType>
const ItemType& LinkedList<ItemType>::getMin() const {
    if (isEmpty()) {
        throw std::runtime_error("List is empty");
    }

    const ListNode* current = headPtr_;
    const ListNode* minNode = current;
    current = current->getLinkToNextNode();

    while (current) {
        if (current->getValue() < minNode->getValue()) {
            minNode = current;
        }
        current = current->getLinkToNextNode();
    }

    return minNode->getValue();
}

// Добавление элемента в голову
template<typename ItemType>
void LinkedList<ItemType>::addToHead(const ItemType& value) {
    ListNode* newNode = new ListNode(value, headPtr_);

    if (headPtr_) {
        headPtr_->setLinkToPrevNode(newNode);
    }

    headPtr_ = newNode;

    if (!tailPtr_) {
        tailPtr_ = newNode;
    }

    ++size_;
}

// Добавление элемента в хвост
template<typename ItemType>
void LinkedList<ItemType>::addToTail(const ItemType& value) {
    ListNode* newNode = new ListNode(value, nullptr, tailPtr_);

    if (tailPtr_) {
        tailPtr_->setLinkToNextNode(newNode);
    }

    tailPtr_ = newNode;

    if (!headPtr_) {
        headPtr_ = newNode;
    }

    ++size_;
}

// Добавление элемента на позицию
template<typename ItemType>
void LinkedList<ItemType>::addAtIndex(uint32_t index, const ItemType& value) {
    if (index > size_) {
        throw std::out_of_range("Index out of range");
    }

    if (index == 0) {
        addToHead(value);
    }
    else if (index == size_) {
        addToTail(value);
    }
    else {
        ListNode* current = getNodeAtIndex(index - 1);
        ListNode* newNode = new ListNode(value, current->getLinkToNextNode(), current);
        current->getLinkToNextNode()->setLinkToPrevNode(newNode);
        current->setLinkToNextNode(newNode);
        ++size_;
    }
}

// Добавление элемента после ключа (после первого вхождения)
template<typename ItemType>
void LinkedList<ItemType>::addAfterKey(const ItemType& key, const ItemType& value) {
    ListNode* current = headPtr_;
    while (current) {
        if (current->getValue() == key) {
            ListNode* newNode = new ListNode(value, current->getLinkToNextNode(), current);

            if (current->getLinkToNextNode()) {
                current->getLinkToNextNode()->setLinkToPrevNode(newNode);
            }
            else {
                tailPtr_ = newNode;
            }

            current->setLinkToNextNode(newNode);
            ++size_;
            return;  // Добавляем после первого вхождения и выходим
        }
        current = current->getLinkToNextNode();
    }
}

// Добавление элемента по итератору (после итератора)
template<typename ItemType>
typename LinkedList<ItemType>::Iterator LinkedList<ItemType>::addAfterIterator(Iterator pos, const ItemType& value) {
    if (pos == end()) {
        // Если итератор указывает на конец, добавляем в хвост
        addToTail(value);
        return Iterator(tailPtr_);
    }

    ListNode* current = pos.getNode();
    ListNode* newNode = new ListNode(value, current->getLinkToNextNode(), current);

    if (current->getLinkToNextNode()) {
        current->getLinkToNextNode()->setLinkToPrevNode(newNode);
    }
    else {
        tailPtr_ = newNode;
    }

    current->setLinkToNextNode(newNode);
    ++size_;

    return Iterator(newNode);
}

// Удаление элемента из головы
template<typename ItemType>
void LinkedList<ItemType>::removeFromHead() {
    if (!headPtr_) return;

    ListNode* temp = headPtr_;
    headPtr_ = headPtr_->getLinkToNextNode();

    if (headPtr_) {
        headPtr_->setLinkToPrevNode(nullptr);
    }
    else {
        tailPtr_ = nullptr;
    }

    delete temp;
    --size_;
}

// Удаление элемента из хвоста
template<typename ItemType>
void LinkedList<ItemType>::removeFromTail() {
    if (!tailPtr_) return;

    ListNode* temp = tailPtr_;
    tailPtr_ = tailPtr_->getLinkToPrevNode();

    if (tailPtr_) {
        tailPtr_->setLinkToNextNode(nullptr);
    }
    else {
        headPtr_ = nullptr;
    }

    delete temp;
    --size_;
}

// Удаление элемента из позиции
template<typename ItemType>
void LinkedList<ItemType>::removeAtIndex(uint32_t index) {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }

    if (index == 0) {
        removeFromHead();
    }
    else if (index == size_ - 1) {
        removeFromTail();
    }
    else {
        ListNode* current = getNodeAtIndex(index);
        current->getLinkToPrevNode()->setLinkToNextNode(current->getLinkToNextNode());
        current->getLinkToNextNode()->setLinkToPrevNode(current->getLinkToPrevNode());
        delete current;
        --size_;
    }
}

// Удаление элемента по ключу (первое вхождение)
template<typename ItemType>
void LinkedList<ItemType>::removeByKey(const ItemType& key) {
    ListNode* current = headPtr_;

    while (current) {
        if (current->getValue() == key) {
            if (current == headPtr_) {
                removeFromHead();
            }
            else if (current == tailPtr_) {
                removeFromTail();
            }
            else {
                current->getLinkToPrevNode()->setLinkToNextNode(current->getLinkToNextNode());
                current->getLinkToNextNode()->setLinkToPrevNode(current->getLinkToPrevNode());
                delete current;
                --size_;
            }
            return;  // Удаляем первое вхождение и выходим
        }
        current = current->getLinkToNextNode();
    }
}

// Удаление элемента по итератору
template<typename ItemType>
typename LinkedList<ItemType>::Iterator LinkedList<ItemType>::removeAtIterator(Iterator pos) {
    ListNode* nodeToRemove = pos.getNode();
    Iterator nextIter(nodeToRemove->getLinkToNextNode());

    if (nodeToRemove == headPtr_) {
        removeFromHead();
    }
    else if (nodeToRemove == tailPtr_) {
        removeFromTail();
    }
    else {
        nodeToRemove->getLinkToPrevNode()->setLinkToNextNode(nodeToRemove->getLinkToNextNode());
        nodeToRemove->getLinkToNextNode()->setLinkToPrevNode(nodeToRemove->getLinkToPrevNode());
        delete nodeToRemove;
        --size_;
    }

    return nextIter;
}

// Удаление диапазона элементов с помощью итераторов [first, last)
template<typename ItemType>
typename LinkedList<ItemType>::Iterator LinkedList<ItemType>::removeRange(Iterator first, Iterator last) {
    if (first == last) return last;

    // Находим граничные узлы
    ListNode* firstNode = first.getNode();
    ListNode* lastNode = last.getNode();

    if (!firstNode) {
        // first - это end()
        return last;
    }

    // Определяем границы для удаления
    ListNode* prevNode = firstNode->getLinkToPrevNode();
    ListNode* nextNode = lastNode ? lastNode : nullptr;

    // Удаляем узлы в диапазоне
    ListNode* current = firstNode;
    uint32_t count = 0;
    while (current != lastNode && current != nullptr) {
        ListNode* next = current->getLinkToNextNode();
        delete current;
        current = next;
        ++count;
    }

    // Обновляем связи
    if (prevNode) {
        prevNode->setLinkToNextNode(nextNode);
    }
    else {
        headPtr_ = nextNode;
    }

    if (nextNode) {
        nextNode->setLinkToPrevNode(prevNode);
    }
    else {
        tailPtr_ = prevNode;
    }

    // Обновляем размер
    size_ -= count;

    return last;
}

// Получение итератора на начало списка
template<typename ItemType>
typename LinkedList<ItemType>::Iterator LinkedList<ItemType>::begin() {
    return Iterator(headPtr_);
}

// Получение константного итератора на начало списка
template<typename ItemType>
typename LinkedList<ItemType>::ConstIterator LinkedList<ItemType>::begin() const {
    return ConstIterator(headPtr_);
}

// Получение итератора на конец списка (позиция после последнего элемента)
template<typename ItemType>
typename LinkedList<ItemType>::Iterator LinkedList<ItemType>::end() {
    return Iterator(nullptr);
}

// Получение константного итератора на конец списка
template<typename ItemType>
typename LinkedList<ItemType>::ConstIterator LinkedList<ItemType>::end() const {
    return ConstIterator(nullptr);
}

// Присваивание (=)
template<typename ItemType>
LinkedList<ItemType>& LinkedList<ItemType>::operator=(const LinkedList<ItemType>& other) {
    if (this != &other) {
        LinkedList<ItemType> temp(other);
        swap(temp);
    }
    return *this;
}

// Получение ссылки на элемент по индексу ([ ])
template<typename ItemType>
ItemType& LinkedList<ItemType>::operator[](uint32_t index) {
    return getNodeAtIndex(index)->getValue();
}

template<typename ItemType>
const ItemType& LinkedList<ItemType>::operator[](uint32_t index) const {
    return getNodeAtIndex(index)->getValue();
}

// Сравнение (==, !=)
template<typename ItemType>
bool LinkedList<ItemType>::operator==(const LinkedList<ItemType>& other) const {
    if (size_ != other.size_) return false;

    ListNode* current1 = headPtr_;
    ListNode* current2 = other.headPtr_;

    while (current1 && current2) {
        if (current1->getValue() != current2->getValue()) {
            return false;
        }
        current1 = current1->getLinkToNextNode();
        current2 = current2->getLinkToNextNode();
    }

    return true;
}

template<typename ItemType>
bool LinkedList<ItemType>::operator!=(const LinkedList<ItemType>& other) const {
    return !(*this == other);
}

// Сложение (конкатенация) списков (+, +=)
template<typename ItemType>
LinkedList<ItemType> LinkedList<ItemType>::operator+(const LinkedList<ItemType>& other) const {
    LinkedList<ItemType> result(*this);
    result += other;
    return result;
}

template<typename ItemType>
LinkedList<ItemType>& LinkedList<ItemType>::operator+=(const LinkedList<ItemType>& other) {
    ListNode* current = other.headPtr_;
    while (current) {
        addToTail(current->getValue());
        current = current->getLinkToNextNode();
    }
    return *this;
}

// Вспомогательный метод для получения узла по индексу
template<typename ItemType>
typename LinkedList<ItemType>::ListNode* LinkedList<ItemType>::getNodeAtIndex(uint32_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }

    ListNode* current = headPtr_;
    for (uint32_t i = 0; i < index; ++i) {
        current = current->getLinkToNextNode();
    }
    return current;
}

// Вспомогательный метод для сортировки слиянием
template<typename ItemType>
typename LinkedList<ItemType>::ListNode* LinkedList<ItemType>::mergeSort(ListNode* head) {
    if (!head || !head->getLinkToNextNode()) {
        return head;
    }

    // Разделяем список на две половины
    ListNode* middle = split(head);
    ListNode* left = head;
    ListNode* right = middle;

    // Рекурсивно сортируем каждую половину
    left = mergeSort(left);
    right = mergeSort(right);

    // Сливаем отсортированные половины
    return merge(left, right);
}

// Вспомогательный метод для разделения списка
template<typename ItemType>
typename LinkedList<ItemType>::ListNode* LinkedList<ItemType>::split(ListNode* head) {
    if (!head || !head->getLinkToNextNode()) {
        return head;
    }

    ListNode* slow = head;
    ListNode* fast = head->getLinkToNextNode();

    while (fast && fast->getLinkToNextNode()) {
        slow = slow->getLinkToNextNode();
        fast = fast->getLinkToNextNode()->getLinkToNextNode();
    }

    ListNode* middle = slow->getLinkToNextNode();
    slow->setLinkToNextNode(nullptr);
    if (middle) {
        middle->setLinkToPrevNode(nullptr);
    }

    return middle;
}

// Вспомогательный метод для слияния двух отсортированных списков
template<typename ItemType>
typename LinkedList<ItemType>::ListNode* LinkedList<ItemType>::merge(ListNode* left, ListNode* right) {
    if (!left) return right;
    if (!right) return left;

    ListNode* result = nullptr;

    if (left->getValue() <= right->getValue()) {
        result = left;
        result->setLinkToNextNode(merge(left->getLinkToNextNode(), right));
    }
    else {
        result = right;
        result->setLinkToNextNode(merge(left, right->getLinkToNextNode()));
    }

    if (result->getLinkToNextNode()) {
        result->getLinkToNextNode()->setLinkToPrevNode(result);
    }
    result->setLinkToPrevNode(nullptr);

    return result;
}

// Реализация класса ListNode

// Конструктор узла списка
template<typename ItemType>
LinkedList<ItemType>::ListNode::ListNode(ItemType value, ListNode* next, ListNode* prev) :
    value_(value), linkToNextNode_(next), linkToPrevNode_(prev) {
}

// Получение значения узла
template<typename ItemType>
ItemType& LinkedList<ItemType>::ListNode::getValue() {
    return value_;
}

// Получение значения узла (константная версия)
template<typename ItemType>
const ItemType& LinkedList<ItemType>::ListNode::getValue() const {
    return value_;
}

// Получение указателя на следующий узел
template<typename ItemType>
typename LinkedList<ItemType>::ListNode* LinkedList<ItemType>::ListNode::getLinkToNextNode() {
    return linkToNextNode_;
}

// Получение указателя на следующий узел (константная версия)
template<typename ItemType>
const typename LinkedList<ItemType>::ListNode* LinkedList<ItemType>::ListNode::getLinkToNextNode() const {
    return linkToNextNode_;
}

// Получение указателя на предыдущий узел
template<typename ItemType>
typename LinkedList<ItemType>::ListNode* LinkedList<ItemType>::ListNode::getLinkToPrevNode() {
    return linkToPrevNode_;
}

// Получение указателя на предыдущий узел (константная версия)
template<typename ItemType>
const typename LinkedList<ItemType>::ListNode* LinkedList<ItemType>::ListNode::getLinkToPrevNode() const {
    return linkToPrevNode_;
}

// Установка указателя на следующий узел
template<typename ItemType>
void LinkedList<ItemType>::ListNode::setLinkToNextNode(ListNode* next) {
    linkToNextNode_ = next;
}

// Установка указателя на предыдущий узел
template<typename ItemType>
void LinkedList<ItemType>::ListNode::setLinkToPrevNode(ListNode* prev) {
    linkToPrevNode_ = prev;
}
