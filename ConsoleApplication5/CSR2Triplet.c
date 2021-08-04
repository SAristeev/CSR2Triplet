// Simple C-converter matrix CSR2Triplet
// CSR format:
// One column - RowPtr, ColInd, Values
// 
// Triplet-format using is used in the library CHOLMOD part of SuiteSparce
// Triplet format:
// Three columns - i,j, A(i,j)

#pragma warning(disable : 4996)
#include <stdio.h>
#include <iostream>
#include <fstream>

void csr2triplet(FILE *aCSR, FILE *aTriplet) {

	int* aRowPtr, * aColInd;
	int a_size, a_nnz;
	double* aVal;

	fscanf(aCSR, "%d", &a_size);
	fscanf(aCSR, "%d", &a_nnz);

	aRowPtr = (int*)malloc(sizeof(int) * (a_size + 1));
	aColInd = (int*)malloc(sizeof(int) * a_nnz);
	aVal = (double*)malloc(sizeof(double) * a_nnz);

	for (int i = 0; i < a_size + 1; ++i) { fscanf(aCSR, "%d", &aRowPtr[i]); }
	for (int i = 0; i < a_nnz; ++i) { fscanf(aCSR, "%d", &aColInd[i]); }
	for (int i = 0; i < a_nnz; ++i) { fscanf(aCSR, "%lf", &aVal[i]); }

	fclose(aCSR);

	fprintf(aTriplet, "%%");
	fprintf(aTriplet, "%%MatrixMarket matrix coordinate real general\n");
	fprintf(aTriplet, "%% Unsymmetric matrix by default\n");
	fprintf(aTriplet, "%d %d %d 0\n", a_size, a_size, a_nnz);

	for (int curRow = 0, i = 0, j = 0; curRow < a_size + 1; ++curRow, ++j) {
		for (int k = 0; k < aRowPtr[j + 1] - aRowPtr[j] && i < a_nnz; ++k, ++i) {
			fprintf(aTriplet, "%d %d %10e\n", curRow, aColInd[i], aVal[i]);
		}
	}

	delete[] aVal;
	delete[] aColInd;
	delete[] aRowPtr;
}

int main() // C for Square Matrix
{
	FILE* aCSR;
	FILE* aTriplet;
	aCSR = fopen("A.txt", "r");
	if (aCSR == NULL) { return -1; }
	aTriplet = fopen("A.tri", "w");

	csr2triplet(aCSR, aTriplet);

	return 0;
}