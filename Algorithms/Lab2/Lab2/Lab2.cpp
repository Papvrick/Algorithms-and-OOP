#include <string>
#include <vector>
#include <iostream>

using namespace std;


// Функция получения первого вхождения подстроки в тексте (алгоритм Бойера-Мура)
int findFirstInd(const string& s, const string& p, int& comparisonCount) {
    comparisonCount = 0; 

    int n = s.size();
    int m = p.size();

    int TAB[256];
    for (int i = 0; i < 256; i++) TAB[i] = m;
    for (int i = 0; i < m - 1; i++) TAB[p[i]] = m - 1 - i;

    int i = m - 1, j = m - 1;
    int k;
    while (i < n && j >= 0) {
        k = i;
        j = m - 1;
        while (j >= 0) {
            comparisonCount++; 
            if (s[k] == p[j]) {
                j--;
                k--;
            }
            else {
                i = i + TAB[s[i]];
                j = m - 1;
                break;
            }
        }
    }
    if (j < 0) return(i + 1 - m);
    else return -1;
}

// Функция получения индексов всех вхождений подстроки в тексте (алгоритм Бойера-Мура)
vector<int> findAllInd(const string& s, const string& p, int& comparisonCount) {
    vector<int> results;
    comparisonCount = 0; 

    int n = s.size();
    int m = p.size();

    int TAB[256];
    for (int i = 0; i < 256; i++) TAB[i] = m;
    for (int i = 0; i < m - 1; i++) TAB[p[i]] = m - 1 - i;

    int i = m - 1, j;
    int k;
    while (i < n) // не конец строки
    {
        k = i;
        j = m - 1;
        while (j >= 0) // j бежит с конца паттерна
        {
            comparisonCount++; 
            if (s[k] == p[j]) // есть совпадение
            {
                j--;
                k--;
            }
            else
            {
                i = i + TAB[s[i]];
                break;
            }
        }
        if (j < 0) // вхождение найдено
        {
            results.push_back(i - m + 1);
            i++; // сдвиг на 1 для поиска следующего вхождения
        }
    }
    return results;
}

// Функция получения индексов вхождений подстроки в тексте в заданном диапазоне (алгоритм Бойера-Мура)
vector<int> findAllInRangeInd(const string& s, const string& p, int start, int end, int& comparisonCount) {
    vector<int> results;
    comparisonCount = 0; 

    if (p.empty() || s.empty() || start > end || start < 0 || end >= s.size()) {
        return results;
    }

    int n = end - start + 1; // длина диапазона
    int m = p.size();

    if (m > n)     // паттерн больше диапазона
    {
        return results;
    }

    int TAB[256];
    for (int i = 0; i < 256; i++) TAB[i] = m;
    for (int i = 0; i < m - 1; i++) TAB[p[i]] = m - 1 - i;

    int i = start + m - 1, j = m - 1; // i - граница начала
    int k;
    while (i < end) // граница конца
    {
        k = i;
        j = m - 1;
        while (j >= 0)
        {
            comparisonCount++; 
            if (s[k] == p[j])
            {
                j--;
                k--;
            }
            else
            {
                i = i + TAB[s[i]];
                break;
            }
        }
        if (j < 0)
        {
            results.push_back(i - m + 1);
            i++;
        }
    }
    return results;
}

// Функция прямого поиска
vector<int> directSearch(const string& s, const string& p, int& comparisonCount) {
    vector<int> results;
    comparisonCount = 0; 

    int n = s.size();
    int m = p.size();

    if (n < m || m == 0) {
        return results;
    }

    // Прямой поиск
    for (int i = 0; i <= n - m; i++) {
        bool found = true;
        for (int j = 0; j < m; j++) {
            comparisonCount++; 
            if (s[i + j] != p[j]) {
                found = false;
                break;
            }
        }
        if (found) {
            results.push_back(i);
        }
    }

    return results;
}

int main()
{
    int first;
    string s, p;
    vector<int> second;
    vector<int> third;

    int comparisonCountDirect = 0;
    int comparisonCountBMFirst = 0;
    int comparisonCountBMAll = 0;
    int comparisonCountBMRange = 0;

    vector<int> directResults;

    cout << "Enter text for comparison: ";
    getline(cin, s);
    cout << endl << "Enter pattern: ";
    getline(cin, p);

    if (p.empty() || s.empty()) {
        cerr << "Error: text or pattern is empty"; return -1;
    }

    if (p.size() > s.size()) {
        cerr << "Error: pattern is longer than text"; return -1;
    }

    int start;
    cout << "Enter the initial search limit (starting with 0): ";
    cin >> start;
    cout << endl;

    if (start < 0) {
        cerr << endl << "Error: incorrect range input";
        return -1;
    }

    int end;
    cout << "Enter the search end boundary(before " << s.size() - 1 << "): ";
    cin >> end;
    cout << endl;

    if (start > end || end >= s.size()) {
        cerr << endl << "Error: incorrect range input";
        return -1;
    }

    // Поиск с использованием алгоритма Бойера-Мура
    first = findFirstInd(s, p, comparisonCountBMFirst);
    second = findAllInd(s, p, comparisonCountBMAll);
    third = findAllInRangeInd(s, p, start, end, comparisonCountBMRange);

    // Поиск с использованием прямого алгоритма
    directResults = directSearch(s, p, comparisonCountDirect);

    // Вывод результатов алгоритма Бойера-Мура
    if (first == -1) {
        cout << "Pattern wasn't found in the text (Boyer-Moore)" << endl;
    }
    else {
        cout << "Pattern was found in the text (Boyer-Moore), the index of the first occurrence: " << first << endl;
    }
    cout << "Number of comparisons for Boyer-Moore (first occurrence): " << comparisonCountBMFirst << endl;

    if (second.size() < 2) {
        cout << "Pattern appears in the text once (Boyer-Moore)" << endl;
    }
    else {
        cout << "Indexes of all occurrences (Boyer-Moore): [ ";
        for (int ind = 0; ind < second.size(); ind++) cout << second[ind] << " ";
        cout << "]" << endl;
    }
    cout << "Number of comparisons for Boyer-Moore (all occurrences): " << comparisonCountBMAll << endl;

    if (third.size() == 0) {
        cout << "There are no occurrences in the specified range (Boyer-Moore)" << endl;
    }
    else {
        cout << "Indexes of occurrences in a given range (Boyer-Moore): [ ";
        for (int ind = 0; ind < third.size(); ind++) cout << third[ind] << " ";
        cout << "]" << endl;
    }
    cout << "Number of comparisons for Boyer-Moore (range search): " << comparisonCountBMRange << endl;

    cout << endl;

    // Вывод результатов прямого поиска
    if (directResults.empty()) {
        cout << "Pattern wasn't found in the text (Direct Search)" << endl;
    }
    else {
        cout << "Pattern was found in the text (Direct Search), the index of the first occurrence: " << directResults[0] << endl;
    }

    if (directResults.size() < 2) {
        cout << "Pattern appears in the text once (Direct Search)" << endl;
    }
    else {
        cout << "Indexes of all occurrences (Direct Search): [ ";
        for (int ind = 0; ind < directResults.size(); ind++) cout << directResults[ind] << " ";
        cout << "]" << endl;
    }

    cout << "Number of comparisons for direct search: " << comparisonCountDirect << endl;

    return 0;
}