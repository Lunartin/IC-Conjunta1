// data_storage.cpp

#include "data_storage.h"
#include <cstdlib>  // Necessário para malloc, free
#include <cstring>  // Necessário para memset

static float* pointsData = NULL;  // Ponteiro para armazenar os dados dos pontos
static int numPoints = 0;         // Número de pontos

void InitData() {
    // Inicializa a memória para os dados dos pontos
    pointsData = NULL;
    numPoints = 0;
}

void SetNumPoints(int numPointsInput) {
    if (numPointsInput < 0) numPointsInput = 0;
    delete[] pointsData; // Libera a memória anterior
    numPoints = numPointsInput;
    pointsData = new float[numPoints * 2](); // Inicializa a memória com zeros
}

void GetTableData(int index, float* x, float* y) {
    if (index < 0 || index >= numPoints) return;
    *x = pointsData[2 * index];
    *y = pointsData[2 * index + 1];
}

void SetTableData(int index, float x, float y) {
    if (index < 0 || index >= numPoints) return;
    pointsData[2 * index] = x;
    pointsData[2 * index + 1] = y;
}

int GetNumPoints() {
    return numPoints;
}

void FreeData() {
    delete[] pointsData; // Libera a memória alocada
    pointsData = NULL;
}
