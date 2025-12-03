#include <iostream>
#include <vector>  
#include <ctime>     
#include <Windows.h>
#include <chrono>
#include <random>
#include <fstream>

using namespace std;

// Счётчики для быстрой сортировки
long long quick_comparisons = 0;
long long quick_swaps = 0;

// Сброс счётчиков быстрой сортировки
void resetQuickSortCounters() {
    quick_comparisons = 0;
    quick_swaps = 0;
}

// Функция быстрой сортировки (алгоритм Хоара)
void quicksort(vector<int>& A, int l, int r) {
    if (l >= r) return;

    int x = A[(l + r) / 2];
    int i = l, j = r;

    while (i <= j) {
        while (i <= r) {
            quick_comparisons++;
            if (A[i] < x) i++;
            else break;
        }

        while (j >= l) {
            quick_comparisons++;
            if (A[j] > x) j--;
            else break;
        }

        if (i <= j) {
            swap(A[i], A[j]);
            quick_swaps++;
            i++;
            j--;
        }
    }

    // Рекурсивные вызовы
    if (l < j) quicksort(A, l, j);
    if (i < r) quicksort(A, i, r);
}

// Функция сортировки пузырьком
pair<long long, long long> bubbleSort(vector<int>& arr) {
    long long bubble_comparisons = 0;
    long long bubble_swaps = 0;

    int n = arr.size();
    bool swapped;

    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            bubble_comparisons++;
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                bubble_swaps++;
                swapped = true;
            }
        }
        // Если на этой итерации не было обменов, массив уже отсортирован
        if (!swapped) break;
    }

    return { bubble_comparisons, bubble_swaps };
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
    if (arr.size() > 100) {
        std::cout << "[массив слишком велик для вывода, размер: " << arr.size() << "]" << std::endl;
        return;
    }

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

// Тестирование быстрой сортировки
pair<double, pair<long long, long long>> testQuickSort(vector<int> testArray) {

    resetQuickSortCounters(); // Сбрасываем счётчики перед тестом

    chrono::high_resolution_clock::time_point timeStart = chrono::high_resolution_clock::now();

    quicksort(testArray, 0, (int)testArray.size() - 1);

    chrono::high_resolution_clock::time_point timeEnd = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = timeEnd - timeStart;

    if (!isSorted(testArray)) {
        cout << "Ошибка: массив не отсортирован!" << endl;
        return { -1.0, {-1, -1} };
    }

    return { duration.count(), {quick_comparisons, quick_swaps} };
}

// Тестирование сортировки пузырьком
pair<double, pair<long long, long long>> testBubbleSort(vector<int> testArray) {
    chrono::high_resolution_clock::time_point timeStart = chrono::high_resolution_clock::now();

    auto bubbleStats = bubbleSort(testArray);

    chrono::high_resolution_clock::time_point timeEnd = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = timeEnd - timeStart;

    if (!isSorted(testArray)) {
        cout << "Ошибка: массив не отсортирован!" << endl;
        return { -1.0, {-1, -1} };
    }

    return { duration.count(), bubbleStats };
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

            // Тестирование быстрой сортировки
            cout << "  Быстрая сортировка (Хоар):" << endl;
            for (int attempt = 1; attempt <= 3; attempt++) {
                auto result = testQuickSort(testArrays[i]);
                if (result.first >= 0) {
                    cout << "    Попытка " << attempt << ": время = " << result.first
                        << " секунд, сравнений = " << result.second.first
                        << ", обменов = " << result.second.second << endl;
                }
            }

            // Тестирование сортировки пузырьком
            cout << "  Сортировка пузырьком:" << endl;
            if (testArrays[i].size() <= 100000) {
                for (int attempt = 1; attempt <= 3; attempt++) {
                    auto result = testBubbleSort(testArrays[i]);
                    if (result.first >= 0) {
                        cout << "    Попытка " << attempt << ": время = " << result.first
                            << " секунд, сравнений = " << result.second.first
                            << ", обменов = " << result.second.second << endl;
                    }
                }
            }
            else {
                cout << "    ПРОПУЩЕНО (массив слишком большой для пузырьковой сортировки)" << endl;
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

        cout << "\nТестирование сортировок (размер: " << size << "):" << endl;

        // Тестирование быстрой сортировки
        cout << "Быстрая сортировка (Хоар):" << endl;
        auto quickResult = testQuickSort(randomArray);
        if (quickResult.first >= 0) {
            cout << "   Время = " << quickResult.first << " секунд" << endl;
            cout << "   Сравнений = " << quickResult.second.first << endl;
            cout << "   Обменов = " << quickResult.second.second << endl;
        }

        // Тестирование сортировки пузырьком
        cout << "\nСортировка пузырьком:" << endl;
        if (size <= 100000) {
            auto bubbleResult = testBubbleSort(randomArray);
            if (bubbleResult.first >= 0) {
                cout << "   Время = " << bubbleResult.first << " секунд" << endl;
                cout << "   Сравнений = " << bubbleResult.second.first << endl;
                cout << "   Обменов = " << bubbleResult.second.second << endl;
            }
        }
        else {
            cout << "   ПРОПУЩЕНО (массив слишком большой для пузырьковой сортировки)" << endl;
        }
    }
    else {
        cout << "Неверный выбор!" << endl;
        return 1;
    }

    return 0;
}