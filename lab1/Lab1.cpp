#include <iostream>
#include <limits.h>
#include <cstdlib>
#include <ctime>

using std::cout;
using std::endl;
using std::min;

// Генерация симметричной матрицы стоимостей маршрутов
void generateCostMatrix(int** graph, int n, int maxCost) {
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; i++) {
			if(i == j) {
				graph[i][j] = 0;
			}
			else if(i < j) {
				graph[i][j] = rand() % maxCost + 1; // Случайная стоимость от 1 до maxCost
				graph[j][i] = graph[i][j];
			}
		}
	}
}

// Вывод матрицы
void printMatrix(int** graph, int n) {
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			cout << graph[i][j] << "\t";
		}
		cout << endl;
	}
}

// Вычисление стоимости пути (точный метод)
int exactTSP(int** graph, bool* path, int pos) {
	if(count == n && graph[pos][startCity]) {
		return cost + graph[pos][startCity];
	}
	int answer = INT_MAX;

	for(int i = 0; i < n; i++) {
		if(graph[pos][i] && !(path[i])) {
			path[i] = true;
			answer = min(answer, exactTSP(graph, path, i, n, startCity, count + 1, cost + graph[pos][i]));
			path[i] = false;
		}
	}
	return answer;
}

// Жадный алгоритм
int greedyTSP(int** graph, int start, int n) {
	bool* path = new bool[n]();
	path[start] = true;
	int totalCost = 0;
	int currentCity = start;

	for(int i = 1; i < n; i++) {
		int nextCity = -1;
		int minCost = INT_MAX;

		for(int j = 0; j < n; j++) {
			if(!path[j] && graph[currentCity][j] < minCost) {
				minCost = graph[currentCity][j];
				nextCity = j;
			}
		}

		totalCost += minCost;
		path[nextCity] = true;
		currentCity = nextCity;
	}

	totalCost += graph[currentCity][start];

	delete[] path;
	return totalCost;
}

int main()
{
	srand(static_cast <unsigned int> (time(0)));
	const int minCities = 3;
	const int maxCities = 14;
	const int maxCost = 100;

	for(int N = minCities; N <= maxCities; N++) {
		// Случайная матрица стоимостей
		int** graph = new int* [N];
		for(int i = 0; i < N; i++) {
			graph[i] = new int[N];
		}

		generateCostMatrix(graph, N, maxCost);

		cout << "The generated cost matrix for " << N << " cities:\n";
		printMatrix(graph, N);

		// Случайный стартовый город
		int startCity = rand() % N;
		cout << "The starting city: " << startCity << endl;

		// Массив посещённых городов
		bool* path = new bool[N]();
		path[startCity] = 1;

		clock_t startTimeExact = clock();
		int exactAnswer = exactTSP(graph, path, startCity, N, startCity);
		clock_t endTimeExact = clock();

		double timeTakenExact = double(endTimeExact - startTimeExact) / CLOCKS_PER_SEC;

		cout << "Exact method: Minimum cost of the path: " << exactAnswer << endl;
		cout << "Exact method: Time taken: " << timeTakenExact << " seconds\n";

		clock_t startTimeGreedy = clock();
		int greedyAnswer = greedyTSP(graph, startCity, N);
		clock_t endTimeGreedy = clock();

		double timeTakenGreedy = double(endTimeGreedy - startTimeGreedy) / CLOCKS_PER_SEC;

		cout << "Greedy method: Minimum cost of the path: " << greedyAnswer << endl;
		cout << "Greedy method: Time taken: " << timeTakenGreedy << " seconds\n\n";

		delete[] path;
		for (int i = 0; i < N; i++) {
			delete[] graph[i];
		}
		delete[] graph;
	}
	
	return 0;
}