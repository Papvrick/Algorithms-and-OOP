#include <string>
#include <vector>
#include <iostream>

using namespace std;


//Функция получения первого вхождения подстроки в тексте
int findFirstInd(const string& s, const string& p) {
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

//Функция получения индексов всех вхождений подстроки в тексте
vector<int> findAllInd(const string& s, const string& p)
{
    vector<int> results;

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

//Функция получения индексов вхождений подстроки в тексте в заданном диапазоне
vector<int> findAllInRangeInd(const string& s, const string& p, int start, int end)
{
    vector<int> results;

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


int main()
{
    int first;
    string s, p;
    vector<int> second;
    vector<int> third;

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

    first = findFirstInd(s, p);
    second = findAllInd(s, p);
    third = findAllInRangeInd(s, p, start, end);

    if (first == -1) {
        cout << "Pattern wasn't found in the text";
        return 0;
    }
    else cout << "Patter was found in the text, the index of the first occurrence: " << first << endl;

    if (second.size() < 2) cout << "Pattern appears in the text once" << endl;
    else {
        cout << "Indexes of all occurrences: [ ";
        for (int ind = 0; ind < second.size(); ind++) cout << second[ind] << " ";
        cout << "]" << endl;
    }

    if (third.size() == 0) cout << "There are no occurrences in the specified range";
    else {
        cout << "Indexes of occurrences in a given range: [ ";
        for (int ind = 0; ind < third.size(); ind++) cout << third[ind] << " ";
        cout << "]" << endl;
    }

    return 0;
}