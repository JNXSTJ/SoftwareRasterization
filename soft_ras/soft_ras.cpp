// soft_ras.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
// 坐标系， 左手坐标系, 和dx一致
// a, b, c, (b - a) x (c - b) 为法线方向

#include <iostream>
#include <vector>
#include <memory>
#include "MatUtil.hpp"

enum CullingState{
    FrontCulling = 0,
    BackCulling = 1,
    NoneCulling = 2
};

enum FillMode {
    FILL_MODE_WIREFRAME, // 线框
    FILL_MODE_SOLID, // 实心
};

typedef struct Color {
    Color() {
        memset(arr, 0, sizeof(arr));
    }
    Color(float r, float g, float b)
    {
        arr[0] = r;
        arr[1] = g;
        arr[2] = b;
        arr[3] = 1.0f;
    }
    Color(float r, float g, float b, float a)
    {
        arr[0] = r;
        arr[1] = g;
        arr[2] = b;
        arr[3] = a;

    }
    float arr[4];
} Color;


class Point
{
public:
    Point() {}
    vector3f Pos() { return pos; }
private:
    vector3f pos;
    float u, v;
};

class Triangle
{
public:
    Triangle() {};
    Triangle(Point a, Point b, Point c):
        a(a), b(b), c(c) {}
    vector3f ComputeNormal()
    {
        return (b.Pos() - a.Pos()).Cross(c.Pos() - b.Pos());
    }
private:
    Point a, b, c;
};

class BackBuffer
{
public:
    BackBuffer() {}
    void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f)
    {
        size_t N = backBuffer.size();
        if (N == 0) return;
        size_t M = backBuffer[0].size();
        for (size_t i = 0; i < N; i++)
        {
            for (size_t j = 0; j < M; j++)
            {
                backBuffer[i][j] = Color(r, g, b);
            }
        }
    }

    void SetPso(CullingState cullingState, FillMode fillMode)
    {
        this->cullingState = cullingState;
        this->fillMode = fillMode;
    }

    void Rasterize(Triangle& tri)
    {
        auto normal = tri.ComputeNormal();
        if (cullingState == CullingState::BackCulling and normal.z() < 0.0f)
        {
            return;
        }
        else if (cullingState == CullingState::FrontCulling and normal.z() > 0.0f)
        {
            return;
        }
        if (fillMode == FillMode::FILL_MODE_SOLID)
        {

        }
        else if (fillMode == FillMode::FILL_MODE_WIREFRAME)
        {

        }
    }
private:
    std::vector<std::vector<Color>> backBuffer;
    CullingState cullingState;
    FillMode fillMode;
};

class Mesh
{
public:
    Mesh(): cullingState(BackCulling), fillMode(FILL_MODE_SOLID)
    {

    }
    bool Load(std::string file_path)
    {
        printf("load mesh file success!");
        return true;
    }
    void SetWorldMatrix(Mat4x4<float>& worldMat)
    {
        this->worldMat = worldMat;
    }
    void SetCullingState(CullingState cullingState)
    {
        this->cullingState = cullingState;
    }
    void SetFillMode(FillMode fillMode)
    {
        this->fillMode = fillMode;
    }
    void Render(std::shared_ptr<BackBuffer> ptr)
    {
        ptr->SetPso(cullingState, fillMode);
        for (auto &tri : triangles)
        {
            ptr->Rasterize(tri);
        }
    }
private:
    Mat4x4<float> worldMat;
    std::vector<Triangle> triangles;
    // pipeline state
    CullingState cullingState;
    FillMode fillMode;
};

class Scene
{
public:
    Scene() {}
    void AddMesh(std::shared_ptr<Mesh> &mesh)
    {
        meshes.push_back(mesh);
    }
    void Render()
    {
        backBuffer->Clear();
        for (auto& mesh : meshes)
        {

        }
    }
private:
    std::shared_ptr<BackBuffer> backBuffer;
    std::vector<std::shared_ptr<Mesh>> meshes;
};

int main()
{
    Mat4x4<float> view_mat, proj_mat;
    std::shared_ptr<Mesh> skull = std::make_shared<Mesh>();
    std::string path = "skull.txt";
    auto ret = skull->Load(path);
    if (ret == false)
    {
        printf("load skull failed!\n");
        exit(1);
    }

    std::shared_ptr<BackBuffer> backBuffer = std::make_shared<BackBuffer>();
    while (true)
    {
        backBuffer->Clear(1.0f, 1.0f, 1.0f);
        skull->Render(backBuffer);
    }
    std::cout << "Hello World!\n";
}

