#pragma once
#include <array>
#include <iostream>
#include <iomanip> // 包含头文件以使用 setw() 和 setprecision()
#include <cassert>

template <typename T>
class Mat4x4;

template <typename T>
class vector4;

template <typename T>
class vector3 {
public:
    typedef std::array<T, 3> EleType;
    vector3() {}
    vector3(T a, T b, T c) : arr{ a, b, c } {}
    vector3(EleType ele) : arr(ele) {}
    vector3(const vector4<T> &o)
    {
        arr[0] = o.x();
        arr[1] = o.y();
        arr[2] = o.z();
    }
    vector3(const vector3& other)
    {
        arr = other.arr;
    }

    T& operator[](const size_t id)
    {
        return arr[id];
    }

    T operator[](const size_t id) const
    {
        return arr[id];
    }

    vector3& operator-=(const vector3& other)
    {
        arr = EleType{ arr[0] - other.arr[0], arr[1] - other.arr[1], arr[2] - other.arr[2] };
        return *this;
    }
    template <typename T>
    vector3<T> operator-(const vector3<T>& other) const
    {
        return EleType{ arr[0] - other.arr[0], arr[1] - other.arr[1], arr[2] - other.arr[2] };
    }
    template <typename T>
    vector3<T> operator/(T other) const
    {
        return { arr[0] / other, arr[1] / other, arr[2] / other };
    }
    template <typename T>
    T operator*(const vector3<T>& other)
    {
        return arr[0] * other.arr[0] + arr[1] * other.arr[1] + arr[2] * other.arr[2];
    }

    vector3 operator-() const
    {
        return { -arr[0], -arr[1], -arr[2] };
    }

    template <typename T>
    vector3<T> Cross(const vector3<T>& other) const
    {
        return EleType{ arr[1] * other.arr[2] - arr[2] * other.arr[1], arr[2] * other.arr[0] - arr[0] * other.arr[2], arr[0] * other.arr[1] - arr[1] * other.arr[0] };
    }

    vector3<T> Normalize()
    {
        auto l = Length();
        arr[0] /= l;
        arr[1] /= l;
        arr[2] /= l;
        return *this;
    }

    T Length()
    {
        if (dirty)
        {
            dirty = false;
            return length = sqrt(arr[0] * arr[0] + arr[1] * arr[1] + arr[2] * arr[2]);
        }
        else return length;
    }

    T& x()
    {
        return arr[0];
    }

    T& y()
    {
        return arr[1];
    }

    T& z()
    {
        return arr[2];
    }

    T x() const
    {
        return arr[0];
    }

    T y() const
    {
        return arr[1];
    }

    T z() const
    {
        return arr[2];
    }
private:
    EleType arr;
    bool dirty;
    T length;
    friend vector4;
};

typedef vector3<float> vector3f;

template <typename T>
class vector4 {
public:
    typedef std::array<T, 4> EleType;
    vector4():dirty(true) {}
    vector4(T x, T y, T z, T w) : arr{ x, y, z, w }, dirty(true) {}
    vector4(EleType ele) : arr(ele), dirty(true) {}
    vector4 (vector3<T> a, T w) {
        this->arr[0] = a[0];
        this->arr[1] = a[1];
        this->arr[2] = a[2];
        this->arr[3] = w;
    }
    vector4& operator-=(const vector4& other)
    {
        arr = EleType{ arr[0] - other.arr[0], arr[1] - other.arr[1], arr[2] - other.arr[2], arr[3] - other.arr[3] };
        return *this;
    }
    template <typename T>
    vector4<T> operator-(const vector4<T>& other)
    {
        return EleType{ arr[0] - other.arr[0], arr[1] - other.arr[1], arr[2] - other.arr[2], arr[3] - other.arr[3] };
    }
    template <typename T>
    vector4<T> operator*(const Mat4x4<T> &matrix)
    {
        return {
            arr[0] * matrix.array[0][0] + arr[1] * matrix.array[1][0] + arr[2] * matrix.array[2][0] + arr[3] * matrix.array[3][0],
            arr[1] * matrix.array[0][1] + arr[1] * matrix.array[1][1] + arr[2] * matrix.array[2][1] + arr[3] * matrix.array[3][1],
            arr[2] * matrix.array[0][2] + arr[1] * matrix.array[1][2] + arr[2] * matrix.array[2][2] + arr[3] * matrix.array[3][2],
            arr[3] * matrix.array[0][3] + arr[1] * matrix.array[1][3] + arr[2] * matrix.array[2][3] + arr[3] * matrix.array[3][3],
        };
    }

    template <typename T>
    vector4<T>& operator*=(const Mat4x4<T> &matrix)
    {
        arr = {
            arr[0] * matrix.array[0][0] + arr[1] * matrix.array[1][0] + arr[2] * matrix.array[2][0] + arr[3] * matrix.array[3][0],
            arr[1] * matrix.array[0][1] + arr[1] * matrix.array[1][1] + arr[2] * matrix.array[2][1] + arr[3] * matrix.array[3][1],
            arr[2] * matrix.array[0][2] + arr[1] * matrix.array[1][2] + arr[2] * matrix.array[2][2] + arr[3] * matrix.array[3][2],
            arr[3] * matrix.array[0][3] + arr[1] * matrix.array[1][3] + arr[2] * matrix.array[2][3] + arr[3] * matrix.array[3][3],
        };
        return *this;
    }

    template <typename T>
    vector4<T>& operator/=(const T f)
    {
        for (int i = 0; i < 4; i++) arr[i] /= f;
        return *this;
    }

    T Length()
    {
        if (dirty)
        {
            dirty = false;
            return length = sqrt(arr[0] * arr[0] + arr[1] * arr[1] + arr[2] * arr[2] + arr[3] * arr[3]);
        }
        else
        {
            return length;
        }
    }
    T& operator[](size_t index)
    {
        assert(index < arr.size());
        return arr[index];
    }
    T& x()
    {
        return arr[0];
    }
    T& y()
    {
        return arr[1];
    }
    T& z()
    {
        return arr[2];
    }
    T& w()
    {
        return arr[3];
    }
    T x() const
    {
        return arr[0];
    }
    T y() const
    {
        return arr[1];
    }
    T z() const
    {
        return arr[2];
    }
    T w() const
    {
        return arr[3];
    }
private:
    EleType arr;
    bool dirty;
    T length;
    friend class Mat4x4<T>;
};

typedef vector4<float> vector4f;

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
class MatrixProxy {
public:
	MatrixProxy(std::array<std::array<T, 4>, 4> *arr, int row) : arr(arr), row(row) {}

	// 二层 [] 操作符重载
	T& operator[](int col) {
		return (*arr)[row][col];
	}

private:
	std::array<std::array<T, 4>, 4>* arr;
	int row;
};


template <typename T>
class Mat4x4
{
public:
    static Mat4x4<T> Identity()
    {
        static Mat4x4<T> identity {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1,
        };
        return identity;
    }
	Mat4x4(std::array<std::array<T, 4>, 4> array);
	Mat4x4(void);
	Mat4x4(T m00, T m01, T m02, T m03,
		T m10, T m11, T m12, T m13,
		T m20, T m21, T m22, T m23,
		T m30, T m31, T m32, T m33);


    // 一层 [] 操作符重载
    MatrixProxy<T> operator[](int row) {
        return MatrixProxy<T>(&array, row);
    }

    void LookAt(vector3<T>& eyePos, vector3<T>& focusPos, vector3<T>& up)
    {
        auto w = focusPos - eyePos;
        w.Normalize();
        auto j = vector3<T>{ 0.0f, 1.0f, 0.0f };
        auto u = j.Cross(w);
        u.Normalize();
        auto v = w.Cross(u);
        this->array = {
            u.x(), v.x(), w.x(), 0.0f,
            u.y(), v.y(), w.y(), 0.0f,
            u.z(), v.z(), w.z(), 0.0f,
            -eyePos * u, -eyePos * v, -eyePos * w, 1.0f
        };
    }


    // 重载<<运算符的友元函数
    friend std::ostream& operator<<(std::ostream& os, const Mat4x4<T>& obj) {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                os << std::setw(10) << std::setprecision(4) << std::fixed << obj.array[i][j] << " ";
            }
            os << std::endl;
        }
        return os;
    }

    template <typename T>
    static Mat4x4<T> SetProjection(T nearPlane, T farPlane, T fovAngleY, T aspect)
    {
        return
        {
            1.0f / aspect / tan(fovAngleY / 2), 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f / tan(fovAngleY / 2), 0.0f, 0.0f,
            0.0f, 0.0f, farPlane / (farPlane - nearPlane), 1.0f,
            0.0f, 0.0f, -farPlane * nearPlane / (farPlane - nearPlane), 0.0f
        };
    }

    template <typename T>
    Mat4x4<T> operator * (const Mat4x4<T>& other) const
    {
		std::array<std::array<T, 4>, 4> c;
        Mat4x4<T> ret;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				float sum = 0.0f;
				for (int k = 0; k < 4; k++)
				{
					sum += array[i][k] * other.array[k][j];
				}
				c[i][j] = sum;
			}
		}
        ret.array = std::move(c);
        return ret;
    }

    template <typename T>
    Mat4x4<T>& operator *=(const Mat4x4<T>& other)
    {
		std::array<std::array<T, 4>, 4> c;
        Mat4x4<T> ret;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				float sum = 0.0f;
				for (int k = 0; k < 4; k++)
				{
					sum += array[i][k] * other.array[k][j];
				}
				c[i][j] = sum;
			}
		}
        array = std::move(c);
        return *this;
    }




private:
	std::array<std::array<T, 4>, 4> array;
    friend class vector4<T>;
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


