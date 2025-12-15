#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;

// Счетчик для пирмамидальной сортировки
long long pyramidComparisons = 0;

// Просеивание вниз 
void siftDown(vector<int>& x, int start, int end) {
    int root = start;
    while (2 * root + 1 <= end) {  // Пока есть хотя бы левый потомок
        int left = 2 * root + 1;
        int right = 2 * root + 2;
        int swapInd = root;        // Начинаем обменивать с корня

        // Сравниваем с левым потомком
        pyramidComparisons++;
        if (x[swapInd] < x[left])
            swapInd = left;

        // Сравниваем с правым потомком (если он существует)
        pyramidComparisons++;
        if (right <= end && x[swapInd] < x[right])
            swapInd = right;

        if (swapInd == root)
            return;

        swap(x[root], x[swapInd]);
        root = swapInd;
    }
}

// Построение пирамиды
void buildPyramid(vector<int>& x) {
    int n = x.size();
    // Просеивание половины массива
    for (int i = n / 2 - 1; i >= 0; i--) {
        siftDown(x, i, n - 1);
    }
}

// Пирамидальная сортировка 
void pyramidSort(vector<int>& x) {
    pyramidComparisons = 0;
    int n = x.size();
    buildPyramid(x);
    for (int k = n - 1; k > 0; k--) {
        swap(x[0], x[k]);
        siftDown(x, 0, k - 1);
    }
}

// Счетчик для прямого выбора
long long selectionComparisons = 0;

// Сортировка прямым выбором
void selectionSort(vector<int>& x) {
    selectionComparisons = 0;
    int n = x.size();

    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;

        // Поиск минимального элемента в неотсортированной части
        for (int j = i + 1; j < n; j++) {
            selectionComparisons++;
            if (x[j] < x[minIndex]) {
                minIndex = j;
            }
        }

        // Обмен местами найденного минимального элемента с первым элементом
        if (minIndex != i) {
            swap(x[i], x[minIndex]);
        }
    }
}

// Проверка на упорядоченность 
bool isSorted(const vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); i++)
        if (arr[i - 1] > arr[i])
            return false;
    return true;
}

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    while (true) {
        vector<int> x, y;
        int rangeStart, rangeEnd, numValues;
        cout << "Введите диапазон случайных чисел (начало и конец): ";
        cin >> rangeStart >> rangeEnd;
        cout << "Введите размер массива(10000 || 100000 || 1000000): ";
        cin >> numValues;

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(rangeStart, rangeEnd);

        x.resize(numValues);
        y.resize(numValues);
        for (int i = 0; i < numValues; i++) {
            int value = dist(gen);
            x[i] = value;
            y[i] = value;
        }

        // Тестирование сортировки прямым выбором
        cout << "\nТестирование сортировки прямым выбором:" << endl;
        chrono::high_resolution_clock::time_point start1 = chrono::high_resolution_clock::now();

        selectionSort(y);

        chrono::high_resolution_clock::time_point end1 = chrono::high_resolution_clock::now();
        chrono::duration<double> duration1 = end1 - start1;

        cout << "   Время сортировки: " << duration1.count() << " секунд" << endl;
        cout << "   Количество сравнений: " << selectionComparisons << endl;
        cout << "   Корректность сортировки: " << (isSorted(y) ? "Да" : "Нет") << endl;

        // Тестирование пирамидальной сортировки
        cout << "\nТестирование пирамидальной сортировка:" << endl;
        chrono::high_resolution_clock::time_point start2 = chrono::high_resolution_clock::now();

        pyramidSort(x);

        chrono::high_resolution_clock::time_point end2 = chrono::high_resolution_clock::now();
        chrono::duration<double> duration2 = end2 - start2;

        cout << "   Время сортировки: " << duration2.count() << " секунд" << endl;
        cout << "   Количество сравнений: " << pyramidComparisons << endl;
        cout << "   Корректность сортировки: " << (isSorted(x) ? "Да" : "Нет") << endl;

        char choice;
        cout << "\nПродолжить? (y/n): ";
        cin >> choice;

        if (choice != 'y' && choice != 'Y') {
            break;
        }
    }

    return 0;
}