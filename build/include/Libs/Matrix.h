#pragma once
#include <array>
#include <iostream>
#include <math.h>
#include <stdexcept>

template <typename Num, uint8_t rows, uint8_t cols>
class Matrix;
template <typename Num, uint8_t rows>
class Vector;
template <typename Num, uint8_t rows, uint8_t cols>
class MatrixBase
{
protected:
    typedef MatrixBase<Num, rows, cols> chain_method;
    friend class Matrix<Num, rows, cols>;
    std::array<std::array<Num, cols>, rows> matrix;

public:
    MatrixBase(initializer_list<initializer_list<Num>> l)
    {
        if (l.size() == rows && (*l.begin()).size() == cols)
        {
            int r = 0;
            int c = 0;
            for (const initializer_list<Num>& i : l)
            {
                for (const Num& x : i)
                {
                    matrix[r][c] = x;
                    c++;
                }
                c = 0;
                r++;
            }
        }
        else
        {
            // throw length_error("Invalid initializer list size");
            cout << "Invalid initializer list size" << endl;
        }
    }
    MatrixBase()
    {
        matrix = std::array<std::array<Num, cols>, rows>();
        for (auto& i : matrix)
        {
            for (auto& x : i)
            {
                x = 0;
            }
        }
    }

    chain_method operator=(initializer_list<initializer_list<Num>> l)
    {
        if (l.size() == rows && (*l.begin()).size() == cols)
        {
            int r = 0;
            int c = 0;
            for (const initializer_list<Num>& i : l)
            {
                for (const Num& x : i)
                {
                    matrix[r][c] = x;
                    c++;
                }
                c = 0;
                r++;
            }
        }
        else
        {
            // throw length_error("Invalid initializer list size");
            cout << "Invalid initializer list size" << endl;
        }

        CHAIN;
    }
    chain_method operator*=(Num scalar)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int y = 0; y < cols; y++)
            {
                matrix[i][y] *= scalar;
            }
        }
        CHAIN;
    }

    Matrix<Num, rows, cols> operator*(Num scalar)
    {
        Matrix<Num, rows, cols> ret;
        for (int x = 0; x < rows; x++)
        {
            for (int y = 0; y < cols; y++)
            {
                ret.matrix[x][y] = matrix[x][y];
            }
        }
        ret *= scalar;
        return ret;
    }
    template <uint8_t c>
    Matrix<Num, rows, c> operator*(Matrix<Num, cols, c>& m)
    {
        Matrix<Num, rows, c> ret;
        for (int i = 0; i < rows; i++)
        {
            auto& row = matrix[i];
            for (int y = 0; y < c; y++)
            {
                Num sum = 0;
                for (int x = 0; x < cols; x++)
                {
                    sum += row[x] * m[x][y];
                }
                ret[i][y] = sum;
            }
        }
        return ret;
    }
    template <uint8_t c>
    Matrix<Num, rows, c> operator*(Matrix<Num, cols, c>&& m)
    {
        return operator*(m);
    }
    template <uint8_t c>
    Matrix<Num, rows, 1> operator*(Vector<Num, c>& m)
    {
        Matrix<Num, rows, 1> ret;
        for (int i = 0; i < rows; i++)
        {
            Num sum = 0;
            auto& row = matrix[i];
            for (int j = 0; j < c; j++)
            {
                sum += row[j] * m[j][0];
            }
            ret[i][0] = sum;
        }
        return ret;
    }
    template <uint8_t c>
    Matrix<Num, rows, 1> operator*(Vector<Num, c>&& m)
    {
        return operator*(m);
    }
    chain_method operator+=(Matrix<Num, rows, cols>& other)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int y = 0; y < cols; y++)
            {
                matrix[i][y] += other.matrix[i][y];
            }
        }
        CHAIN;
    }
    chain_method operator+=(Matrix<Num, rows, cols>&& other)
    {
        return operator+=(other);
    }
    Matrix<Num, rows, cols> operator+(Matrix<Num, rows, cols>& other)
    {
        Matrix<Num, rows, cols> ret;
        for (int x = 0; x < rows; x++)
        {
            for (int y = 0; y < cols; y++)
            {
                ret.matrix[x][y] = matrix[x][y];
            }
        }
        ret += other;
        return ret;
    }
    Matrix<Num, rows, cols> operator+(Matrix<Num, rows, cols>&& other)
    {
        return operator+(other);
    }
    Matrix<Num, rows, cols> operator-(Matrix<Num, rows, cols>& other)
    {
        Matrix<Num, rows, cols> ret;
        for (int x = 0; x < rows; x++)
        {
            for (int y = 0; y < cols; y++)
            {
                ret.matrix[x][y] = matrix[x][y];
            }
        }
        ret -= other;
        return ret;
    }
    Matrix<Num, rows, cols> operator-(Matrix<Num, rows, cols>&& other)
    {
        return operator-(other);
    }
    chain_method operator-=(Matrix<Num, rows, cols>& other)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int y = 0; y < cols; y++)
            {
                matrix[i][y] -= other.matrix[i][y];
            }
        }
        CHAIN;
    }
    chain_method operator-=(Matrix<Num, rows, cols>&& other)
    {
        return operator-=(other);
    }

    Matrix<Num, cols, rows> transpose()
    {
        Matrix<Num, cols, rows> ret;
        for (int i = 0; i < rows; i++)
        {
            for (int y = 0; y < cols; y++)
            {
                ret[y][i] = matrix[i][y];
            }
        }
        return ret;
    }
    std::array<Num, cols>& operator[](int i)
    {
        return matrix[i];
    }
    Num& operator()(int x, int y)
    {
        return matrix[x][y];
    }

    typename std::array<std::array<Num, cols>, rows>::iterator begin()
    {
        return matrix.begin();
    }
    typename std::array<std::array<Num, cols>, rows>::iterator end()
    {
        return matrix.end();
    }
};
template <typename Num, uint8_t rows, uint8_t cols>
class Matrix : public MatrixBase<Num, rows, cols>
{
    friend class MatrixBase<Num, rows, cols>;

public:
    Matrix(initializer_list<initializer_list<Num>> l) : MatrixBase<Num, rows, cols>(l)
    {
    }
    Matrix() : MatrixBase<Num, rows, cols>()
    {
    }
};
template <typename Num>
class Matrix<Num, 1, 1> : public MatrixBase<Num, 1, 1>
{
    static constexpr uint8_t cols = 1;
    static constexpr uint8_t rows = 1;
    using MatrixBase<Num, rows, cols>::matrix;
    friend class MatrixBase<Num, rows, cols>;

public:
    Matrix(initializer_list<initializer_list<Num>> l) : MatrixBase<Num, rows, rows>(l)
    {
    }
    Matrix() : MatrixBase<Num, rows, cols>()
    {
    }
    static Matrix<Num, rows, cols> Identity()
    {
        Matrix<Num, rows, cols> ret;
        int x = 0;
        int y = 0;
        while (x < rows)
        {
            ret[x][y] = 1;
            x++;
            y++;
        }
        return ret;
    }
    double determinant()
    {
        return matrix[0][0];
    }
    Matrix<Num, rows, cols> adjoint()
    {
        return {{1}};
    }
    Matrix<Num, rows, cols> inverse()
    {
        Matrix<Num, rows, cols> ret;
        auto adj = adjoint();
        auto det = determinant();
        if (det == 0)
        {
            return ret;
        }
        adj *= 1 / det;
        if (rows % 2 == 0)
        {
            for (auto& l : adj.matrix)
            {
                for (int i = 1; i < rows; i += 2)
                {
                    l[i] *= -1;
                }
            }
        }
        ret = adj;
        return ret;
    }
};
template <typename Num, uint8_t rows>
class Matrix<Num, rows, rows> : public MatrixBase<Num, rows, rows>
{
    static constexpr uint8_t cols = rows;
    using MatrixBase<Num, rows, cols>::matrix;
    friend class MatrixBase<Num, rows, cols>;

public:
    Matrix(initializer_list<initializer_list<Num>> l) : MatrixBase<Num, rows, rows>(l)
    {
    }
    Matrix() : MatrixBase<Num, rows, cols>()
    {
    }
    static Matrix<Num, rows, cols> Identity()
    {
        Matrix<Num, rows, cols> ret;
        int x = 0;
        int y = 0;
        while (x < rows)
        {
            ret[x][y] = 1;
            x++;
            y++;
        }
        return ret;
    }
    double determinant()
    {
        Num ret = 0.0;
        if (rows == 1)
        {
            return matrix[0][0];
        }
        else if (rows == 2)
        {
            return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
        }
        int sign = 1;
        for (int i = 0; i < cols; i++)
        {
            Num val = matrix[0][i];
            Matrix<Num, rows - 1, cols - 1> smaller;
            for (int x = 1; x < rows; x++)
            {
                int j = 0;
                for (int y = 0; y < cols; y++)
                {
                    if (y == i)
                    {
                        continue;
                    }
                    smaller[x - 1][j] = matrix[x][y];
                    j++;
                }
            }
            auto d = smaller.determinant();
            ret += val * (Num)sign * d;
            sign *= -1;
        }
        return ret;
    }
    Matrix<Num, rows, cols> adjoint()
    {
        Matrix<Num, rows, cols> ret;
        Matrix<Num, rows - 1, cols - 1> smaller;
        int sign = 1;
        if (rows == 1)
        {
            return {{1}};
        }
        for (int ix = 0; ix < rows; ix++)
        {
            for (int iy = 0; iy < rows; iy++)
            {
                int xs = 0;

                for (int x = 0; x < rows; x++)
                {
                    if (x == ix)
                    {
                        continue;
                    }
                    int ys = 0;
                    for (int y = 0; y < cols; y++)
                    {
                        if (y == iy)
                        {
                            continue;
                        }
                        smaller[xs][ys] = matrix[x][y];
                        ys++;
                    }
                    xs++;
                }
                ret[ix][iy] = sign * smaller.determinant();
                sign *= -1;
            }
        }
        return ret.transpose();
    }
    Matrix<Num, rows, cols> inverse()
    {
        Matrix<Num, rows, cols> ret;
        auto adj = adjoint();
        auto det = determinant();
        if (det == 0)
        {
            return ret;
        }
        adj *= 1 / det;
        if (rows % 2 == 0)
        {
            for (auto& l : adj.matrix)
            {
                for (int i = 1; i < rows; i += 2)
                {
                    l[i] *= -1;
                }
            }
        }
        ret = adj;
        return ret;
    }
};
template <typename Num, uint8_t rows>
class Vector : public MatrixBase<Num, rows, 1>
{
    static constexpr uint8_t cols = 1;
    using MatrixBase<Num, rows, cols>::matrix;
    friend class MatrixBase<Num, rows, cols>;
    typedef Vector<Num, rows> chain_method;

public:
    Vector(initializer_list<Num> l)
    {
        int i = 0;
        for (auto& n : l)
        {
            matrix[i][0] = n;
            i++;
        }
    }
    Vector() : MatrixBase<Num, rows, cols>()
    {
    }
    chain_method operator=(initializer_list<Num> l)
    {
        if (l.size() == rows)
        {
            int c = 0;
            for (const Num& i : l)
            {
                matrix[c][0] = i;
                c++;
            }
        }
        else
        {
            // throw length_error("Invalid initializer list size");
            cout << "Invalid initializer list size" << endl;
        }

        CHAIN;
    }
    operator Matrix<Num, rows, 1>()
    {
        Matrix<Num, rows, 1> ret;
        ret.matrix = matrix;
        return ret;
    }
};
template <typename Num, uint8_t r, uint8_t c>
ostream& operator<<(ostream& cout, Matrix<Num, r, c>& m)
{
    auto p = m;
    for (auto& i : p)
    {
        for (auto& x : i)
        {
            cout << (abs(x) < 1e-14 ? 0 : x) << ", ";
        }
        cout << endl;
    }
    return cout;
}
template <typename Num, uint8_t r, uint8_t c>
ostream& operator<<(ostream& cout, Matrix<Num, r, c>&& m)
{
    return operator<<(cout, m);
}
template <typename Num, uint8_t r>
ostream& operator<<(ostream& cout, Vector<Num, r>& m)
{
    auto p = m;
    for (auto& i : p)
    {
        for (auto& x : i)
        {
            cout << (abs(x) < 1e-14 ? 0 : x) << ", ";
        }
        cout << endl;
    }
    return cout;
}
template <typename Num, uint8_t r>
ostream& operator<<(ostream& cout, Vector<Num, r>&& m)
{
    return operator<<(cout, m);
}
