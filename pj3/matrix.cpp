#include "matrix.h"
#include <cstring>
#include <iostream>

using namespace std;

Matrix::Matrix(int rv, int cv){
    r = rv;
    c = cv;
    arr = new float*[r];
    for (int i = 0; i < r; i++) {
        arr[i] = new float[c];
        for (int j = 0; j < r; j++)
            arr[i][j] = 0;
    }
}

Matrix::Matrix(const Matrix& rhs) {
    if (this != &rhs) {
        r = rhs.r;
        c = rhs.c;
        arr = new float*[r];
        for (int i = 0; i < r; i++) {
            arr[i] = new float[c];
            memcpy(arr[i], rhs.arr[i], sizeof(float) * c);
        }
    }
}

Matrix& Matrix::operator=(const Matrix& rhs) {
    if (this != &rhs) {
        r = rhs.r;
        c = rhs.c;
        for (int i = 0; i < r; i++)
            delete[] arr[r];
        delete[] arr;
        arr = new float*[r];
        for (int i = 0; i < r; i++) {
            arr[i] = new float[c];
            memcpy(arr[i], rhs.arr[i], sizeof(float) * c);
        }
    }
}


float *& Matrix::operator[] (const int nIndex) {
    return arr[nIndex];
}

Matrix::~Matrix() {
    for (int i = 0; i < r; i++)
        delete[] arr[i];
    delete[] arr;
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
    Matrix ret(lhs.r, rhs.c);
    for (int i = 0; i < lhs.r; i++)
        for (int j = 0; j < rhs.c; j++)
            for (int k = 0; k < lhs.c; k++)
                ret.arr[i][j] += lhs.arr[i][k] * rhs.arr[k][j];
    return ret;
}
