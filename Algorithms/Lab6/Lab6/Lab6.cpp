#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <chrono>
#include <string>
#include <ctime>

using namespace std;

// Генерация случайного массива
vector<int> generateRandomArray(int size, int min_val, int max_val) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distrib(min_val, max_val);

    vector<int> arr(size);
    for (int i = 0; i < size; i++) {
        arr[i] = distrib(gen);
    }
    return arr;
}

// Вывод массива
void outputArray(const vector<int>& arr) {
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
}

// Проверка на отсортированность
bool isSorted(vector<int>& arr) {
    for (int i = 1; i < arr.size(); i++) {
        if (arr[i] < arr[i - 1]) {
            return false;
        }
    }
    return true;
}

// Получение k-го бита числа
int getBit(int num, int k) {
    unsigned int unsigned_num = static_cast<unsigned int>(num);
    return (unsigned_num >> k) & 1;
}

// Рекурсивная функция побитовой сортировки
void bitwiseSort(vector<int>& arr, int l, int r, int k) {
    if (l >= r || k < 0) {
        return;
    }

    int i = l;
    int j = r;

    // Разделяем отрицательные и положительные числа ("-" - влево, "+" - вправо)
    if (k == sizeof(int) * 8 - 1) {
        // "-" число - бит = 1
        // "+" число - бит = 0
        while (i <= j) {
            while (i <= j && getBit(arr[i], k) == 1) { // "-"
                i++;
            }
            while (i <= j && getBit(arr[j], k) == 0) { // "+"
                j--;
            }
            if (i < j) {
                swap(arr[i], arr[j]);
                i++;
                j--;
            }
        }
    }
    else {
        // Для остальных битов - обычный алгоритм
        while (i <= j) {
            while (i <= j && getBit(arr[i], k) == 0) {
                i++;
            }

            while (i <= j && getBit(arr[j], k) == 1) {
                j--;
            }

            if (i < j) {
                swap(arr[i], arr[j]);
                i++;
                j--;
            }
        }
    }

    // Рекурсивные вызовы
    bitwiseSort(arr, l, j, k - 1);
    bitwiseSort(arr, i, r, k - 1);
}

// Чтение массива из файла
vector<int> readArrayFromFile(const string& filename) {
    vector<int> arr;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Ошибка: не удалось открыть файл " << filename << endl;
        return arr;
    }

    int value;
    while (file >> value) {
        arr.push_back(value);
    }

    file.close();
    return arr;
}

// Чтение всех массивов из списка файлов
vector<vector<int>> readArraysFromFiles(const vector<string>& filenames) {
    vector<vector<int>> arrays;

    for (size_t i = 0; i < filenames.size(); ++i) {
        vector<int> arr = readArrayFromFile(filenames[i]);
        if (!arr.empty()) {
            arrays.push_back(arr);
            cout << "Прочитан массив из " << filenames[i] << " (размер: " << arr.size() << ")" << endl;
        }
    }

    return arrays;
}

// Тестирование побитовой сортировки 
double testBitSort(vector<int>& arr) {
    chrono::high_resolution_clock::time_point timeStart = chrono::high_resolution_clock::now();

    if (!arr.empty()) {
        int max_bit = sizeof(int) * 8 - 1;
        bitwiseSort(arr, 0, (int)arr.size() - 1, max_bit);
    }

    chrono::high_resolution_clock::time_point timeEnd = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = timeEnd - timeStart;

    if (!isSorted(arr)) {
        cout << "Ошибка: массив не отсортирован!" << endl;
    }

    return duration.count();
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    srand(time(0));

    int choice;

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

        vector<vector<int>> testArrays = readArraysFromFiles(filenames);

        if (testArrays.empty()) {
            cout << "Ошибка: не удалось прочитать массивы из файлов" << endl;
            return 1;
        }

        for (size_t i = 0; i < testArrays.size(); ++i) {
            cout << "\nТестовый массив " << i + 1 << " (размер: " << testArrays[i].size() << "):" << endl;
            for (int attempt = 1; attempt <= 3; attempt++) {
                double time = testBitSort(testArrays[i]);
                cout << "  Попытка " << attempt << ": время = " << time << " секунд" << endl;
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
        outputArray(randomArray);

        cout << "Тестирование случайного массива (размер: " << size << "):" << endl;
        double time = testBitSort(randomArray);
        cout << "Время = " << time << " секунд" << endl;

        cout << "Отсортированный массив: ";
        outputArray(randomArray);
    }
    else {
        cout << "Неверный выбор!" << endl;
        return 1;
    }

    return 0;
}