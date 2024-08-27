// data_storage.h

#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H

void InitData();
void SetNumPoints(int numPointsInput);
void GetTableData(int index, float *x, float *y);
void SetTableData(int index, float x, float y);
int GetNumPoints();
void FreeData(); // Adiciona a declaração da função

#endif // DATA_STORAGE_H
