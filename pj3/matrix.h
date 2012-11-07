#ifndef MATRIX_H
#define MATRIX_H

class Matrix {
private:
    float **arr;
public:
    int r, c;
    Matrix(int rv, int cv);
    Matrix(const Matrix& rhs);
    Matrix& operator=(const Matrix& rhs);
    ~Matrix();

    friend Matrix operator*(const Matrix& l, const Matrix& r);
    float *& operator[] (const int nIndex);
};

#endif
