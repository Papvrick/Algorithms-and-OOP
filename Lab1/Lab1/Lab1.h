#pragma once
void generateCostMatrix(int** graph, int n, int maxCost);
void printMatrix(int** graph, int n);
int exactTSP(int** graph, bool* path, int pos, int n, int startCity, int count = 1, int cost = 0);
int greedyTSP(int** graph, int start, int n);