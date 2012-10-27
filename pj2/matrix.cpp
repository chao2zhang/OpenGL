#include "matrix.h"

Matrix::Matrix(int rv, int cv): r(rv), c(cv) {
    arr = new int*[r];
    for (int i = 0; i < r; i++)
        arr[r] = new int[c];
}

Matrix::~Matrix() {
    for (int i = 0; i < r; i++)
        delete[] arr[r];
    delete[] arr;
}
