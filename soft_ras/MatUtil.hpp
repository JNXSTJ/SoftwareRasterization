#pragma once
#include <array>
#include <iostream>
#include <cassert>

template <typename T>
class vector3 {
public:
    typedef std::array<T, 3> EleType;
    vector3() {}
    vector3(T a, T b, T c) : arr{ a, b, c } {}
    vector3(EleType ele) : arr(ele) {}
    vector3& operator-=(const vector3& other)
    {
        arr = EleType{ arr[0] - other.arr[0], arr[1] - other.arr[1], arr[2] - other.arr[2] };
        return *this;
    }
    template <typename T>
    vector3<T> operator-(const vector3<T>& other)
    {
        return EleType{ arr[0] - other.arr[0], arr[1] - other.arr[1], arr[2] - other.arr[2] };
    }
    template <typename T>
    vector3<T> Cross(const vector3<T>& other)
    {
        return EleType{ arr[1] * other.arr[2] - arr[2] * other.arr[1], arr[2] * other.arr[0] - arr[0] * other.arr[2], arr[0] * other.arr[1] - arr[1] * other.arr[0] };
    }
    T operator[](size_t index)
    {
        assert(index < 3);
        return arr[index];
    }
    T x()
    {
        return arr[0];
    }
    T y()
    {
        return arr[1];
    }
    T z()
    {
        return arr[2];
    }
private:
    EleType arr;
};

typedef vector3<float> vector3f;

template <typename T, size_t N, size_t M>
std::array<std::array<T, N>, M>
Transpose(std::array<std::array<T, M>, N> array)
{
    std::array<std::array<T, N>, M> ret;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            ret[j][i] = array[i][j];
        }
    }
    return ret;
}

template <typename T, size_t N, size_t M>
void PrintArray(std::array<std::array<T, M>, N> array)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            std::cout << array[i][j] << " ";
        }
        std::cout << std::endl;
    }
}


template <typename T, size_t N>
std::array<std::array<float, N - 1>, N - 1>
SubArray(std::array<std::array<T, N>, N>& array, int x, int y)
{
    std::array<std::array<float, N - 1>, N - 1> ret;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (i == x || j == y) continue;
            ret[i > x ? i - 1 : i][j > y ? j - 1 : j] = array[i][j];
        }
    }
    return ret;
}

template <typename T>
float Determinent(std::array<std::array<T, 2>, 2> array)
{
    return array[0][0] * array[1][1] - array[0][1] * array[1][0];
}

template <typename T>
float Determinent(std::array<std::array<T, 1>, 1> array)
{
    return array[0][0];
}

template <typename T, size_t N>
float Determinent(std::array<std::array<T, N>, N> array)
{
    static_assert(N >= 2 && N <= 4, "N must be between 2 and 4");
    float ret = 0.0f;
    float flag = 1.0f;
    for (int i = 0; i < N; i++)
    {
        auto tmp = SubArray(array, 0, i);
        float v = Determinent(tmp);
        ret += flag * v * array[0][i];
        flag = -flag;
    }
    return ret;
}

template <typename T, size_t N>
std::array<std::array<T, N>, N> Inverse(std::array<std::array<T, N>, N>& array)
{
    std::array<std::array<T, N>, N> C_T;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            C_T[j][i] = ((i + j) % 2 == 0 ? 1 : -1) * Determinent(SubArray(array, i, j));
        }
    }
    float det = Determinent(array);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            C_T[i][j] /= det;
        }
    }

    return C_T;
}

template <typename T, size_t N>
std::array<std::array<T, N>, N> MatMul(std::array<std::array<T, N>, N>& a, std::array<std::array<T, N>, N> b)
{
    std::array<std::array<T, N>, N> c;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            float sum = 0.0f;
            for (int k = 0; k < N; k++)
            {
                sum += a[i][k] * b[k][j];
            }
            c[i][j] = sum;
        }
    }
    return c;
}



template <typename T>
class Mat4x4
{
public:
	Mat4x4(std::array<std::array<T, 4>, 4> array);
	Mat4x4(void);
	Mat4x4(T m00, T m01, T m02, T m03,
		T m10, T m11, T m12, T m13,
		T m20, T m21, T m22, T m23,
		T m30, T m31, T m32, T m33);

private:
	std::array<std::array<T, 4>, 4> array;
};

template <typename T>
Mat4x4<T>::Mat4x4(std::array<std::array<T, 4>, 4> array)
{
    this->array = array;
}

template <typename T>
Mat4x4<T>::Mat4x4(void)
{
    this->array = { {T(0)}};
}

template <typename T>
Mat4x4<T>::Mat4x4(T m00, T m01, T m02, T m03,
		T m10, T m11, T m12, T m13,
		T m20, T m21, T m22, T m23,
		T m30, T m31, T m32, T m33)
{
    this->array = { {m00, m01, m02, m03,
        m10, m11, m12, m13,
        m20, m21, m22, m23,
        m30, m31, m32, m33} };
}


