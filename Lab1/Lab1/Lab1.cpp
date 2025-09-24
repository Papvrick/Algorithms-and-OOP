#include <iostream>
#include <limits.h>
#include <cstdlib>
#include <ctime>

using std::cout;
using std::cin;
using std::endl;
using std::min;

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

// глобальные переменные для хранения путей
int bestCost = INT_MAX;
int worstCost = 0;
int* bestPath = nullptr;
int* worstPath = nullptr;

// точный метод
void exactTSP(int** graph, bool* visited, int* currPath, int pos, int n, int startCity, int count, int cost) {
    if (count == n) {
        cost += graph[pos][startCity]; // возврат в начало
        currPath[count] = startCity;   // сохранить возврат в путь

        if (cost < bestCost) {
            bestCost = cost;
            for (int i = 0; i <= n; i++) bestPath[i] = currPath[i];
        }
        if (cost > worstCost) {
            worstCost = cost;
            for (int i = 0; i <= n; i++) worstPath[i] = currPath[i];
        }
        return;
    }

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            visited[i] = true;
            currPath[count] = i;
            exactTSP(graph, visited, currPath, i, n, startCity, count + 1, cost + graph[pos][i]);
            visited[i] = false;
        }
    }
}

// Жадный алгоритм
int greedyTSP(int** graph, int start, int n, int* greedyPath) {
    bool* path = new bool[n]();
    path[start] = true;
    greedyPath[0] = start;
    int totalCost = 0;
    int currentCity = start;

    for (int i = 1; i < n; i++) {
        int nextCity = -1;
        int minCost = INT_MAX;

        for (int j = 0; j < n; j++) {
            if (!path[j] && graph[currentCity][j] < minCost) {
                minCost = graph[currentCity][j];
                nextCity = j;
            }
        }

        totalCost += minCost;
        path[nextCity] = true;
        greedyPath[i] = nextCity;
        currentCity = nextCity;
    }

    totalCost += graph[currentCity][start]; // возврат
    greedyPath[n] = start;                  // сохранить возврат в путь
    delete[] path;
    return totalCost;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    const int maxCost = 100;

    int N;
    char choice;

    do {
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

        int startCity = rand() % N;
        cout << "The starting city: " << startCity + 1 << endl;

        // Массивы для точного метода
        bool* visited = new bool[N]();
        int* currPath = new int[N + 1];
        bestPath = new int[N + 1];
        worstPath = new int[N + 1];

        visited[startCity] = true;
        currPath[0] = startCity;

        cout << "\n   Exact method   " << endl;
        clock_t startTimeExact = clock();
        exactTSP(graph, visited, currPath, startCity, N, startCity, 1, 0);
        clock_t endTimeExact = clock();
        double timeTakenExact = double(endTimeExact - startTimeExact) / CLOCKS_PER_SEC;

        cout << "Best cost: " << bestCost << endl;
        cout << "Best path: ";
        printPath(bestPath, N);

        cout << "Worst cost: " << worstCost << endl;
        cout << "Worst path: ";
        printPath(worstPath, N);

        cout << "Time taken: " << timeTakenExact << " seconds" << endl;

        // Жадный алгоритм
        int* greedyPathArr = new int[N + 1];
        cout << "\n   Greedy method   " << endl;
        clock_t startTimeGreedy = clock();
        int greedyAnswer = greedyTSP(graph, startCity, N, greedyPathArr);
        clock_t endTimeGreedy = clock();
        double timeTakenGreedy = double(endTimeGreedy - startTimeGreedy) / CLOCKS_PER_SEC;

        cout << "Greedy cost: " << greedyAnswer << endl;
        cout << "Greedy path: ";
        printPath(greedyPathArr, N);
        cout << "Time taken: " << timeTakenGreedy << " seconds" << endl;

        delete[] visited;
        delete[] currPath;
        delete[] bestPath;
        delete[] worstPath;
        delete[] greedyPathArr;

        for (int i = 0; i < N; i++) {
            delete[] graph[i];
        }
        delete[] graph;

        bestCost = INT_MAX;
        worstCost = 0;

        cout << "\nDo you want to try with another number of cities? (y/n): ";
        cin >> choice;

    } while (choice == 'y' || choice == 'Y');

    return 0;
}
