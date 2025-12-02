#include <iostream>
#include <vector>  
#include <ctime>     
#include <Windows.h>
#include <chrono>
#include <random>
#include <fstream>

using namespace std;

// Функция быстрой сортировки (алгоритм Хоара)
void quicksort(vector<int>& A, int l, int r) {
    if (l >= r) return;

    int x = A[(l + r) / 2];
    int i = l, j = r;
    while (i <= j) {
        while (A[i] < x) i++;
        while (A[j] > x) j--;
        if (i <= j) {
            swap(A[i], A[j]);
            i++;
            j--;
        }
    }
    // Рекурсивные вызовы
    if (l < j) quicksort(A, l, j);
    if (i < r) quicksort(A, i, r);
}

// Функция для чтения массива из файла
std::vector<int> readArrayFromFile(const std::string& filename) {
    std::vector<int> arr;
    FILE* file = NULL;
    errno_t err = fopen_s(&file, filename.c_str(), "r");

    if (file == NULL) {
        std::cout << "Ошибка: не удалось открыть файл " << filename << std::endl;
        return arr;
    }

    int value;
    while (fscanf_s(file, "%d", &value) == 1) {
        arr.push_back(value);
    }

    fclose(file);
    return arr;
}

// Функция для чтения всех массивов из списка файлов
std::vector<std::vector<int>> readArraysFromFiles(const std::vector<std::string>& filenames) {
    std::vector<std::vector<int>> arrays;

    for (size_t i = 0; i < filenames.size(); ++i) {
        std::vector<int> arr = readArrayFromFile(filenames[i]);
        if (!arr.empty()) {
            arrays.push_back(arr);
            std::cout << "Прочитан массив из " << filenames[i] << " (размер: " << arr.size() << ")" << std::endl;
        }
    }

    return arrays;
}

// Функция для вывода массива
void printArray(const std::vector<int>& arr) {
    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

// Функция проверки отсортированности массива
bool isSorted(const std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < arr[i - 1]) {
            return false;
        }
    }
    return true;
}

// Функция генерации случайного массива
std::vector<int> generateRandomArray(int size, int min_val, int max_val) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(min_val, max_val);

    std::vector<int> arr(size);
    for (int i = 0; i < size; i++) {
        arr[i] = distrib(gen);
    }
    return arr;
}

// Тестирование
double testSort(const vector<int>& testArray) {
    vector<int> arr = testArray; // создание копии для тестирования

    chrono::high_resolution_clock::time_point timeStart = chrono::high_resolution_clock::now();

    // Вызов быстрой сортировки
    quicksort(arr, 0, (int)arr.size() - 1);

    chrono::high_resolution_clock::time_point timeEnd = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = timeEnd - timeStart;

    if (!isSorted(arr)) {
        cout << "Ошибка: массив не отсортирован!" << endl;
        return -1.0;
    }

    return duration.count();
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    srand(time(0));

    int choice;
    vector<vector<int>> testArrays;

    cout << "Выберите способ ввода данных: 1 - файлы с массивами, 2 - случайный массив:" << endl;
    cin >> choice;

    if (choice == 1) {
        vector<std::string> filenames = {
            "array_10000_-10_10.txt",
            "array_10000_-1000_1000.txt",
            "array_10000_-100000_100000.txt",
            "array_100000_-10_10.txt",
            "array_100000_-1000_1000.txt",
            "array_100000_-100000_100000.txt",
            "array_1000000_-10_10.txt",
            "array_1000000_-1000_1000.txt",
            "array_1000000_-100000_100000.txt",
        };

        testArrays = readArraysFromFiles(filenames);

        if (testArrays.empty()) {
            cout << "Ошибка: не удалось прочитать массив из файлов" << endl;
            return 1;
        }

        for (size_t i = 0; i < testArrays.size(); ++i) {
            cout << "Тестовый массив " << i + 1 << " (размер: " << testArrays[i].size() << "):" << endl;
    
            for (int attempt = 1; attempt <= 3; attempt++) {
                double time = testSort(testArrays[i]);
                if (time >= 0) {
                    cout << "  Попытка " << attempt << ": время = " << time << " секунд" << endl;
                }
            }
            cout << endl;
        }
    }
    else if (choice == 2) {
        int size, min_val, max_val;
        cout << "Введите размер массива: ";
        cin >> size;
        cout << "Введите минимальное значение: ";
        cin >> min_val;
        cout << "Введите максимальное значение: ";
        cin >> max_val;

        if (size <= 0) {
            cout << "Некорректный размер массива!" << endl;
            return 1;
        }

        vector<int> randomArray = generateRandomArray(size, min_val, max_val);

        cout << "Сгенерированный массив: ";
        printArray(randomArray);

        cout << "Тестирование случайного массива (размер: " << size << "):" << endl;
        double time = testSort(randomArray);
        if (time >= 0) {
            cout << "Время = " << time << " секунд" << endl;
        }

        cout << "Отсортированный массив: ";
        printArray(randomArray);
    }
    else {
        cout << "Неверный выбор!" << endl;
        return 1;
    }

    return 0;
}