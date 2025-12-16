#include "linkedlist.h"
#include <iostream>
#include <cassert>
#include <Windows.h>

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // Тест 1: Конструкторы
    std::cout << " Тест 1: Конструкторы " << std::endl;
    LinkedList<int> list1;
    std::cout << "Пустой список: " << list1 << std::endl;

    int arr[] = { 1, 2, 3, 4, 5 };
    LinkedList<int> list2(arr, 5);
    std::cout << "Список из массива: " << list2 << std::endl;

    LinkedList<int> list3(list2);
    std::cout << "Копия списка: " << list3 << std::endl;

    // Тест 2: Размер и пустота
    std::cout << "\n Тест 2: Размер и пустота " << std::endl;
    std::cout << "Размер list2: " << list2.getSize() << std::endl;
    std::cout << "list1 пуст: " << (list1.isEmpty() ? "да" : "нет") << std::endl;
    std::cout << "list2 пуст: " << (list2.isEmpty() ? "да" : "нет") << std::endl;

    // Тест 3: Добавление элементов
    std::cout << "\n Тест 3: Добавление элементов " << std::endl;
    LinkedList<int> list4;
    list4.addToHead(3);
    list4.addToHead(2);
    list4.addToHead(1);
    std::cout << "После addToHead: " << list4 << std::endl;

    list4.addToTail(4);
    list4.addToTail(5);
    std::cout << "После addToTail: " << list4 << std::endl;

    list4.addAtIndex(2, 99);
    std::cout << "После addAtIndex(2, 99): " << list4 << std::endl;

    list4.addAfterKey(99, 100);
    std::cout << "После addAfterKey(99, 100): " << list4 << std::endl;

    // Тест 4: Удаление элементов
    std::cout << "\n  Тест 4: Удаление элементов " << std::endl;
    list4.removeFromHead();
    std::cout << "После removeFromHead: " << list4 << std::endl;

    list4.removeFromTail();
    std::cout << "После removeFromTail: " << list4 << std::endl;

    list4.removeAtIndex(1);
    std::cout << "После removeAtIndex(1): " << list4 << std::endl;

    list4.removeByKey(100);
    std::cout << "После removeByKey(100): " << list4 << std::endl;

    // Тест 5: Поиск и доступ
    std::cout << "\n Тест 5: Поиск и доступ " << std::endl;
    int* found = list2.find(3);
    std::cout << "find(3) в list2: " << (found ? *found : 0) << std::endl;

    std::cout << "list2[2]: " << list2[2] << std::endl;
    std::cout << "Максимум list2: " << list2.getMax() << std::endl;
    std::cout << "Минимум list2: " << list2.getMin() << std::endl;

    // Тест 6: Итераторы
    std::cout << "\n Тест 6: Итераторы " << std::endl;
    std::cout << "list2 через итераторы: ";
    for (auto it = list2.begin(); it != list2.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // Добавление по итератору
    auto it = list2.begin();
    ++it; // Второй элемент
    list2.addAfterIterator(it, 99);
    std::cout << "После addAfterIterator (добавили 99 после второго): " << list2 << std::endl;

    // Удаление по итератору
    it = list2.begin();
    ++it; // Теперь это 99
    list2.removeAtIterator(it);
    std::cout << "После removeAtIterator (удалили 99): " << list2 << std::endl;

    // Удаление диапазона
    LinkedList<int> list5;
    for (int i = 1; i <= 10; i++) list5.addToTail(i);
    std::cout << "list5: " << list5 << std::endl;

    auto first = list5.begin();
    auto last = list5.begin();
    for (int i = 0; i < 3; i++) ++first;
    for (int i = 0; i < 7; i++) ++last;
    list5.removeRange(first, last);
    std::cout << "После removeRange(3, 7): " << list5 << std::endl;

    // Тест 7: Сортировка
    std::cout << "\n Тест 7: Сортировка " << std::endl;
    LinkedList<int> unsorted;
    unsorted.addToTail(5);
    unsorted.addToTail(1);
    unsorted.addToTail(9);
    unsorted.addToTail(3);
    unsorted.addToTail(7);
    std::cout << "До сортировки: " << unsorted << std::endl;
    unsorted.sort();
    std::cout << "После сортировки: " << unsorted << std::endl;

    // Тест 8: Операторы
    std::cout << "\n Тест 8: Операторы " << std::endl;
    LinkedList<int> listA;
    listA.addToTail(1);
    listA.addToTail(2);

    LinkedList<int> listB;
    listB.addToTail(3);
    listB.addToTail(4);

    LinkedList<int> listC = listA + listB;
    std::cout << "listA + listB: " << listC << std::endl;

    listA += listB;
    std::cout << "listA += listB: " << listA << std::endl;

    std::cout << "listA == listC: " << (listA == listC ? "да" : "нет") << std::endl;

    // Тест 9: Очистка
    std::cout << "\n Тест 9: Очистка " << std::endl;
    std::cout << "listC до clear: " << listC << ", размер: " << listC.getSize() << std::endl;
    listC.clear();
    std::cout << "listC после clear: " << listC << ", размер: " << listC.getSize() << std::endl;

    // Тест 10: Обмен
    std::cout << "\n Тест 10: Обмен " << std::endl;
    LinkedList<int> swapA;
    swapA.addToTail(10);
    swapA.addToTail(20);

    LinkedList<int> swapB;
    swapB.addToTail(30);
    swapB.addToTail(40);
    swapB.addToTail(50);

    std::cout << "До swap - swapA: " << swapA << ", swapB: " << swapB << std::endl;
    swapA.swap(swapB);
    std::cout << "После swap - swapA: " << swapA << ", swapB: " << swapB << std::endl;

    return 0;
}