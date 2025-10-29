#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <string>

using namespace std;

// ����������� ���� 
void siftDown(vector<int>& x, int start, int end) {
    int root = start;
    while (2 * root + 1 <= end) {  // ���� ���� ���� �� ����� �������
        int left = 2 * root + 1;
        int right = 2 * root + 2;
        int swapInd = root;        // �������� ���������� � �����

        // ���������� � ����� ��������
        if (x[swapInd] < x[left])
            swapInd = left;

        // ���������� � ������ �������� (���� �� ����������)
        if (right <= end && x[swapInd] < x[right])
            swapInd = right;

        if (swapInd == root)
            return;

        swap(x[root], x[swapInd]);
        root = swapInd;
    }
}


// 1) ���������� ��������
void buildPyramid(vector<int>& x) {
    int n = x.size();
    // ����������� �������� �������
    for (int i = n / 2 - 1; i >= 0; i--) {
        siftDown(x, i, n - 1);
    }
}

// 2) ���������� 
void pyramidSort(vector<int>& x) {
    int n = x.size();
    buildPyramid(x);
    for (int k = n - 1; k > 0; k--) {        // k - ������ ���������� �������� � ��� �� ��������������� ����� ������� 
        swap(x[0], x[k]);
        siftDown(x, 0, k - 1);
    }
}

// �������� �� ��������������� 
bool isSorted(const vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); i++)
        if (arr[i - 1] > arr[i])
            return false;
    return true;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    while (true) {

        vector<int> x;
        int rangeStart, rangeEnd, numValues;
        cout << "������� �������� ��������� ����� (������ � �����): ";
        cin >> rangeStart >> rangeEnd;
        cout << "������� ������ �������(10000 || 100000 || 1000000): ";
        cin >> numValues;

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(rangeStart, rangeEnd);

        x.resize(numValues);
        for (int i = 0; i < numValues; i++) {
            x[i] = dist(gen);
        }

#if 0
        // ����� ��������� �������
        cout << "\n��������������� ������:\n";
        for (int i = 0; i < x.size(); i++) {
            cout << x[i] << " ";
        }
        cout << "\n";

#endif
        // ����� ������� � ����������
        chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();

        pyramidSort(x);

        chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();

        chrono::duration<double> duration = end - start;
        cout << "\n����� ����������: " << duration.count() << " ������";

#if 0
        // ����� ���������������� �������
        cout << "\n��������������� ������:\n";
        for (int i = 0; i < x.size(); i++) {
            cout << x[i] << " ";
        }
#endif
        cout << "\n";
        cout << "\n";


        cout << "������������ �� ������? ";
        if (isSorted(x))
            cout << "��";
        else
            cout << "���";
        cout << endl;

        char choice;
        cout << "\n����������? (y/n): ";
        cin >> choice;

        if (choice != 'y' && choice != 'Y') {
            break;
        }
    }

    return 0;
}