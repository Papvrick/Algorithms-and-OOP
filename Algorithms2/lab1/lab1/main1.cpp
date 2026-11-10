#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <random>
#include <filesystem>
#include <chrono>
#include <windows.h>

namespace fs = std::filesystem;

const std::string RUN_END = "#";

struct RunReader {
    std::ifstream stream;
    long long current = 0;
    bool has_element = false;

    bool open(const std::string& filename) {
        stream.open(filename);
        has_element = false;
        return stream.is_open();
    }

    void close() {
        if (stream.is_open()) {
            stream.close();
        }
        has_element = false;
    }

    bool advanceInRun() {
        std::string token;
        if (stream >> token) {
            if (token == RUN_END) {
                has_element = false;
                return false;
            }
            current = std::stoll(token);
            has_element = true;
            return true;
        }
        has_element = false;
        return false;
    }
};

struct QueueNode {
    long long val;
    size_t file_index;

    bool operator>(const QueueNode& other) const {
        return val > other.val;
    }
};

size_t getElementsCount(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) return 0;
    size_t count = 0;
    long long val;
    while (file >> val) {
        count++;
    }
    return count;
}

bool isFileContainsSortedArray(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл для проверки: " << fileName << std::endl;
        return false;
    }

    long long prev;
    if (!(file >> prev)) {
        return true;
    }

    long long current;
    while (file >> current) {
        if (current < prev) {
            return false;
        }
        prev = current;
    }
    return true;
}

void printFilePreview(const std::string& fileName, size_t previewLimit = 10) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cout << "  [Не удалось открыть файл]" << std::endl;
        return;
    }

    std::vector<long long> head;
    std::vector<long long> tail;
    long long val;
    size_t total = 0;

    while (file >> val) {
        total++;
        if (head.size() < previewLimit) {
            head.push_back(val);
        }
        else {
            if (tail.size() >= previewLimit) {
                tail.erase(tail.begin());
            }
            tail.push_back(val);
        }
    }

    if (total == 0) {
        std::cout << "  [Пустой файл]" << std::endl;
        return;
    }

    std::cout << "  (Элементов: " << total << ") -> ";
    if (total <= previewLimit * 2) {
        for (long long x : head) std::cout << x << " ";
        for (long long x : tail) std::cout << x << " ";
    }
    else {
        for (long long x : head) std::cout << x << " ";
        std::cout << "... ";
        for (long long x : tail) std::cout << x << " ";
    }
    std::cout << std::endl;
}

bool createFileWithNumbers(const std::string& fileName, const int numbersCount, const int maxNumberValue) {
    std::ofstream file(fileName);
    if (!file.is_open()) return false;

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<long long> dist(-maxNumberValue, maxNumberValue);

    for (int i = 0; i < numbersCount; ++i) {
        file << dist(gen) << (i + 1 == numbersCount ? "" : " ");
    }
    return true;
}

// Многофазная сортировка
void sortFile(const std::string& fileName, size_t numFiles = 3) {
    if (numFiles < 3) {
        numFiles = 3;
    }

    const size_t p = numFiles - 1; // Порядок слияния

    std::vector<std::string> tempNames(numFiles);
    for (size_t i = 0; i < numFiles; ++i) {
        tempNames[i] = "poly_tmp_" + std::to_string(i) + ".tmp";
    }

    //  1. Распределение исходных серий
  
    std::ifstream src(fileName);
    if (!src.is_open()) return;

    long long current_val;
    if (!(src >> current_val)) {
        src.close();
        return;
    }

    // Логическая адресация файлов: f[0..p-1] — входы, f[p] — выход
    std::vector<size_t> f(numFiles);
    for (size_t i = 0; i < numFiles; ++i) f[i] = i;

    std::vector<std::ofstream> outFiles(p);
    for (size_t i = 0; i < p; ++i) {
        outFiles[i].open(tempNames[f[i]]);
    }

    std::vector<long long> a(p, 1);
    std::vector<long long> d(p, 1);
    long long level = 1;
    size_t j = 0;

    outFiles[j] << current_val << " ";
    d[j]--;

    long long prev_val = current_val;
    while (src >> current_val) {
        if (current_val < prev_val) {
            outFiles[j] << RUN_END << " ";

            if (j + 1 < p && d[j] < d[j + 1]) {
                j++;
            }
            else if (d[j] == 0) {
                level++;
                long long z = a[0];
                for (size_t i = 0; i < p - 1; ++i) {
                    d[i] = z + a[i + 1] - a[i];
                    a[i] = z + a[i + 1];
                }
                d[p - 1] = z - a[p - 1];
                a[p - 1] = z;
                j = 0;
            }
            else {
                j = 0;
            }
            d[j]--;
        }

        outFiles[j] << current_val << " ";
        prev_val = current_val;
    }
    outFiles[j] << RUN_END << " ";

    src.close();
    for (size_t i = 0; i < p; ++i) {
        outFiles[i].close();
    }

    // 2.Многофазное слияние 
    
    while (level > 0) {
        long long z = a[p - 1]; // Количество слияний на данном шаге

        std::vector<RunReader> readers(p);
        for (size_t i = 0; i < p; ++i) {
            readers[i].open(tempNames[f[i]]);
        }

        std::ofstream target(tempNames[f[p]], std::ios::trunc);
        long long d_out = 0;

        for (long long m = 0; m < z; ++m) {
            bool all_dummy = true;
            for (size_t i = 0; i < p; ++i) {
                if (d[i] == 0) {
                    all_dummy = false;
                    break;
                }
            }

            if (all_dummy) {
                for (size_t i = 0; i < p; ++i) d[i]--;
                d_out++;
                continue;
            }

            std::priority_queue<QueueNode, std::vector<QueueNode>, std::greater<QueueNode>> pq;

            for (size_t i = 0; i < p; ++i) {
                if (d[i] > 0) {
                    d[i]--;
                }
                else {
                    if (readers[i].advanceInRun()) {
                        pq.push({ readers[i].current, i });
                    }
                }
            }

            while (!pq.empty()) {
                QueueNode top = pq.top();
                pq.pop();

                target << top.val << " ";

                size_t src_idx = top.file_index;
                if (readers[src_idx].advanceInRun()) {
                    pq.push({ readers[src_idx].current, src_idx });
                }
            }

            if (level > 1) {
                target << RUN_END << " ";
            }
        }

        target.close();

        // Потоковый перенос остатков (непрочитанных серий) без загрузки в память
        for (size_t i = 0; i < p - 1; ++i) {
            std::string remName = "rem_" + std::to_string(i) + ".tmp";
            std::ofstream remOut(remName, std::ios::trunc);
            std::string token;
            while (readers[i].stream >> token) {
                remOut << token << " ";
            }
            remOut.close();
            readers[i].close();

            fs::remove(tempNames[f[i]]);
            fs::rename(remName, tempNames[f[i]]);
        }
        readers[p - 1].close();

        // Опустошаем истощенный файл f[p-1]
        {
            std::ofstream clearDepleted(tempNames[f[p - 1]], std::ios::trunc);
        }

        // Циклическая ротация файлов
        std::vector<size_t> new_f(numFiles);
        new_f[0] = f[p];
        for (size_t i = 0; i < p - 1; ++i) {
            new_f[i + 1] = f[i];
        }
        new_f[p] = f[p - 1];
        f = new_f;

        // Циклическая ротация серий a и d
        std::vector<long long> new_a(p, 0);
        std::vector<long long> new_d(p, 0);
        new_a[0] = z;
        new_d[0] = d_out;
        for (size_t i = 1; i < p; ++i) {
            new_a[i] = a[i - 1] - z;
            new_d[i] = d[i - 1];
        }
        a = new_a;
        d = new_d;

        level--;
    }

    // Итоговый отсортированный результат находится в файле f[0]
    fs::remove(fileName);
    fs::rename(tempNames[f[0]], fileName);

    // Удаление временных файлов
    for (size_t i = 0; i < numFiles; ++i) {
        if (fs::exists(tempNames[i])) {
            fs::remove(tempNames[i]);
        }
    }
}

int createAndSortFile(const std::string& fileName, const int numbersCount, const int maxNumberValue) {
    if (!createFileWithNumbers(fileName, numbersCount, maxNumberValue)) {
        return -1;
    }

    std::cout << "\nФайл до сортировки (" << fileName << "):" << std::endl;
    printFilePreview(fileName);

    sortFile(fileName);

    std::cout << "Файл после сортировки (" << fileName << "):" << std::endl;
    printFilePreview(fileName);

    if (!isFileContainsSortedArray(fileName)) {
        return -2;
    }
    return 1;
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    std::vector<std::string> sampleFiles = {
        "array_10000_-10_10.txt",
        "array_10000_-1000_1000.txt",
        "array_10000_-100000_100000.txt",
        "array_100000_-10_10.txt",
        "array_100000_-1000_1000.txt",
        "array_100000_-100000_100000.txt",
        "array_1000000_-10_10.txt",
        "array_1000000_-1000_1000.txt",
        "array_1000000_-100000_100000.txt"
    };

    const size_t auxiliaryFiles = 4; // Можно передать любое число >= 3

    std::cout << "Тестирование файлов из задания" << std::endl;
    for (const auto& sample : sampleFiles) {
        if (!fs::exists(sample)) {
            continue;
        }

        std::string workCopy = "work_" + sample;
        fs::copy_file(sample, workCopy, fs::copy_options::overwrite_existing);

        size_t countBefore = getElementsCount(workCopy);

        std::cout << "\n-------------------------------------------" << std::endl;
        std::cout << "Файл: " << sample << std::endl;
        std::cout << "До сортировки:" << std::endl;
        printFilePreview(workCopy);

        auto start = std::chrono::high_resolution_clock::now();
        sortFile(workCopy, auxiliaryFiles);
        auto end = std::chrono::high_resolution_clock::now();

        size_t countAfter = getElementsCount(workCopy);

        std::cout << "После сортировки:" << std::endl;
        printFilePreview(workCopy);

        std::chrono::duration<double> sec = end - start;
        bool isSorted = isFileContainsSortedArray(workCopy);

        if (isSorted && countBefore == countAfter) {
            std::cout << "Результат: УСПЕХ (" << sec.count() << " c), элементы сохранены: "
                << countAfter << "/" << countBefore << std::endl;
        }
        else {
            std::cout << "Результат: ОШИБКА! Отсортирован: " << (isSorted ? "ДА" : "НЕТ")
                << ", элементов осталось: " << countAfter << " из " << countBefore << std::endl;
        }

        fs::remove(workCopy);
    }

    std::cout << "\n=== Тест через createAndSortFile ===" << std::endl;
    std::string dynamicTestFile = "test_array.txt";
    int res = createAndSortFile(dynamicTestFile, 25, 100);
    if (res == 1) {
        std::cout << "Тест прошёл успешно" << std::endl;
    }
    else {
        std::cout << "Тест провален (код: " << res << ")" << std::endl;
    }
    fs::remove(dynamicTestFile);

    return 0;
}