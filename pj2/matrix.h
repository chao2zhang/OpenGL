#ifndef MATRIX_H
#define MATRIX_H

class Matrix {
public:
    int r, c;
    int **arr;
    Matrix(int rv, int cv);
    ~Matrix();
};

#endif
