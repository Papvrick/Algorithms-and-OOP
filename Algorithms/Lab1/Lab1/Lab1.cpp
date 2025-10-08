#include <iostream>
#include <limits.h>
#include <cstdlib>
#include <ctime>

using std::cout;
using std::cin;
using std::endl;

// Генерация симметричной матрицы стоимостей маршрутов
void generateCostMatrix(int** graph, int n, int maxCost) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                graph[i][j] = 0;
            }
            else if (i < j) {
                graph[i][j] = rand() % maxCost + 1;
                graph[j][i] = graph[i][j];
            }
        }
    }
}

// Вывод матрицы
void printMatrix(int** graph, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << graph[i][j] << "\t";
        }
        cout << endl;
    }
}

// Печать пути
void printPath(int* path, int n) {
    for (int i = 0; i <= n; i++) {
        cout << path[i] + 1;
        if (i < n) cout << ", ";
    }
    cout << endl;
}

// Алгоритм Дейкстры
bool Permutation(int* P, int n) {
    int i = n - 2;
    while (i >= 0 && P[i] >= P[i + 1]) {
        i--;
    }
    if (i < 0) return false; 

    int j = n - 1;
    while (P[j] <= P[i]) {
        j--;
    }

    int temp = P[i];
    P[i] = P[j];
    P[j] = temp;

    int l = i + 1, r = n - 1;
    while (l < r) {
        int tmp = P[l];
        P[l] = P[r];
        P[r] = tmp;
        l++;
        r--;
    }
    return true;
}

// Жадный алгоритм
int greedyTSP(int** graph, int start, int n, int* greedyPath) {
    bool* visited = new bool[n]();
    visited[start] = true;
    greedyPath[0] = start;
    int totalCost = 0;
    int currentCity = start;

    for (int i = 1; i < n; i++) {
        int nextCity = -1;
        int minCost = INT_MAX;

        for (int j = 0; j < n; j++) {
            if (!visited[j] && graph[currentCity][j] < minCost) {
                minCost = graph[currentCity][j];
                nextCity = j;
            }
        }

        totalCost += minCost;
        visited[nextCity] = true;
        greedyPath[i] = nextCity;
        currentCity = nextCity;
    }

    totalCost += graph[currentCity][start];
    greedyPath[n] = start;
    delete[] visited;
    return totalCost;
}

// Наилучишй/Наихудший путь
void bestOrWorstWay(int** graph, int N, int startCity, int& bestCost, int& worstCost, int* bestPath, int* worstPath) {
    int* perm = new int[N - 1];
    int idx = 0;
    for (int i = 0; i < N; i++) {
        if (i != startCity) perm[idx++] = i;
    }

    bestCost = INT_MAX;
    worstCost = 0;

    do {
        int cost = 0;
        int prev = startCity;

        // от стартового до остальных городов
        for (int i = 0; i < N - 1; i++) {
            cost += graph[prev][perm[i]];
            prev = perm[i];
        }

        cost += graph[prev][startCity]; // возврат

        if (cost < bestCost) {
            bestCost = cost;
            bestPath[0] = startCity;
            for (int i = 0; i < N - 1; i++) bestPath[i + 1] = perm[i];
            bestPath[N] = startCity;
        }
        if (cost > worstCost) {
            worstCost = cost;
            worstPath[0] = startCity;
            for (int i = 0; i < N - 1; i++) worstPath[i + 1] = perm[i];
            worstPath[N] = startCity;
        }

    } while (Permutation(perm, N - 1));

    delete[] perm;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    const int maxCost = 100;

    int N;
    cout << "Enter the number of cities: ";
    cin >> N;

    // Создание матрицы стоимостей
    int** graph = new int* [N];
    for (int i = 0; i < N; i++) {
        graph[i] = new int[N];
    }

    generateCostMatrix(graph, N, maxCost);

    cout << "\nGenerated cost matrix for " << N << " cities:\n";
    printMatrix(graph, N);

    char choice;
    do {
        int startCity;
        cout << "\nEnter starting city (1.." << N << "): ";
        cin >> startCity;
        startCity--;

        // точный метод 
        int* bestPath = new int[N + 1];
        int* worstPath = new int[N + 1];
        int bestCost, worstCost;

        clock_t startTimeExact = clock();
        bestOrWorstWay(graph, N, startCity, bestCost, worstCost, bestPath, worstPath);
        clock_t endTimeExact = clock();
        double timeTakenExact = double(endTimeExact - startTimeExact) / CLOCKS_PER_SEC;

        cout << "\nExact method (start = " << startCity + 1 << "):\n";
        cout << "Best cost: " << bestCost << endl;
        cout << "Best path: ";
        printPath(bestPath, N);

        cout << "Worst cost: " << worstCost << endl;
        cout << "Worst path: ";
        printPath(worstPath, N);

        cout << "Time taken: " << timeTakenExact << " seconds" << endl;

        // жадный алгоритм
        int* greedyPathArr = new int[N + 1];
        clock_t startTimeGreedy = clock();
        int greedyAnswer = greedyTSP(graph, startCity, N, greedyPathArr);
        clock_t endTimeGreedy = clock();
        double timeTakenGreedy = double(endTimeGreedy - startTimeGreedy) / CLOCKS_PER_SEC;

        cout << "\nGreedy algorithm (start = " << startCity + 1 << "):\n";
        cout << "Greedy cost: " << greedyAnswer << endl;
        cout << "Greedy path: ";
        printPath(greedyPathArr, N);
        cout << "Time taken: " << timeTakenGreedy << " seconds" << endl;

        delete[] bestPath;
        delete[] worstPath;
        delete[] greedyPathArr;

        cout << "\nTry another starting city? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    for (int i = 0; i < N; i++) {
        delete[] graph[i];
    }
    delete[] graph;

    return 0;
}
    