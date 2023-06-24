#pragma once

#include <vector>
#include <memory>
#include <cmath>
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

struct Pso
{
    Pso(): cullingState(CullingState::BackCulling), fillMode(FillMode::FILL_MODE_SOLID) {}
    CullingState cullingState;
    FillMode fillMode;
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
    Point(const Point& p)
    {
        pos = p.pos;
        u = p.u;
        v = p.v;
    }
    Point(float x, float y, float z): pos(x, y, z), u(0.0f), v(0.0f) {}
    Point(vector3f &pos): pos(pos), u(0.0f), v(0.0f)
    {
    }
    Point(vector3f &pos, float u, float v): pos(pos), u(u), v(v)
    {
    }
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
    std::vector<vector4f> Data() {
        return {
            {a.Pos(), 1.0f},
            {b.Pos(), 1.0f},
            {c.Pos(), 1.0f}
        };
    }
private:
    Point a, b, c;
};

static bool insideTriangle(int x, int y, const vector4f* _v)
{
    // TODO : Implement this function to check if the point (x, y) is inside the triangle represented by _v[0], _v[1], _v[2]
    vector3f t(x, y, 0);
    vector3f a(_v[0]);
    vector3f b(_v[1]);
    vector3f c(_v[2]);
    a.z() = 0;
    b.z() = 0;
    c.z() = 0;
    float t1 = (b - a).Cross(t - a).z();
    float t2 = (c - b).Cross(t - b).z();
    float t3 = (a - c).Cross(t - c).z();
    return t1 >= 0 && t2 >= 0 && t3 >= 0 || t1 <= 0 && t2 <= 0 && t3 <= 0;
}

static std::tuple<float, float, float> computeBarycentric2D(float x, float y, const vector4f* v)
{
    float c1 = (x * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * y + v[1].x() * v[2].y() - v[2].x() * v[1].y()) / (v[0].x() * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * v[0].y() + v[1].x() * v[2].y() - v[2].x() * v[1].y());
    float c2 = (x * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * y + v[2].x() * v[0].y() - v[0].x() * v[2].y()) / (v[1].x() * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * v[1].y() + v[2].x() * v[0].y() - v[0].x() * v[2].y());
    float c3 = (x * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * y + v[0].x() * v[1].y() - v[1].x() * v[0].y()) / (v[2].x() * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * v[2].y() + v[0].x() * v[1].y() - v[1].x() * v[0].y());
    return { c1,c2,c3 };
}


class BackBuffer
{
public:
    BackBuffer() {}
    BackBuffer(int height, int width):
        height(height), width(width)
    {
        colorBuffer = std::vector<Color>(height * width, {0, 0, 0, 0});
        depthBuffer.resize(height * width);
    }
    void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f)
    {
        std::fill(depthBuffer.begin(), depthBuffer.end(), std::numeric_limits<float>::infinity());
        std::fill(colorBuffer.begin(), colorBuffer.end(), Color{ r,g,b });
    }

    std::vector<Color>& Data()
    {
        return colorBuffer;
    }

    int GetId(int x, int y)
    {
        return (height - 1 - y) * width + x;
    }

    void SetPso(const Pso &pso)
    {
        this->pso = pso;
    }

    void Rasterize(Triangle tri, Mat4x4<float> &mvp)
    {
        auto normal = tri.ComputeNormal();
        auto data = tri.Data();
        for (auto& d : data)
        {
            d *= mvp;
            d /= d.w();
            d.x() = width * (1 + d.x()) / 2.0f;
            d.y() = height * (1 + d.y()) / 2.0f;
        }
        //if (pso.cullingState == CullingState::BackCulling && normal.z() < 0.0f)
        //{
        //    return;
        //}
        //else if (pso.cullingState == CullingState::FrontCulling && normal.z() > 0.0f)
        //{
        //    return;
        //}
        if (pso.fillMode == FillMode::FILL_MODE_WIREFRAME)
        {

        }
        else if (pso.fillMode == FillMode::FILL_MODE_SOLID)
        {
            float min_x = width, min_y = height, max_x = 0.0f, max_y = 0.0f;
            for (int i = 0; i < 3; i++)
            {
                min_x = std::min(min_x, data[i].x());
                max_x = std::max(max_x, data[i].x());
                min_y = std::min(min_y, data[i].y());
                max_y = std::max(max_y, data[i].y());
            }

            for (int x = min_x; x <= max_x; x++)
            {
                for (int y = min_y; y <= max_y; y++)
                {
                    if (insideTriangle(x, y, data.data()))
                    {
                        auto [alpha, beta, gamma] = computeBarycentric2D(x, y, data.data());
                        float w_reciprocal = 1.0 / (alpha / data[0].w() + beta / data[1].w() + gamma / data[2].w());
                        float z_interpolated = alpha * data[0].z() / data[0].w() + beta * data[1].z() / data[1].w() + gamma * data[2].z() / data[2].w();
                        z_interpolated *= w_reciprocal;
                        int id = GetId(x, y);
                        if (id < 0 || id >= depthBuffer.size()) continue;
                        if (z_interpolated >= depthBuffer[id]) continue;
                        depthBuffer[id] = z_interpolated;
                        colorBuffer[id] = { 255.0f, 0.0f, 0.0f, 1.0f };
                    }
                }
            }

        }
    }

    void DrawLine(Triangle& tri)
    {
         
    }
private:
    std::vector<Color> colorBuffer;
    std::vector<float> depthBuffer;
    int width, height;
    Pso pso;
};

class Mesh
{
public:
    Mesh()
    {
        worldMat = Mat4x4<float>::Identity();
    }

    bool Load(std::string file_path)
    {
        printf("load mesh file success!");
        return true;
    }

    bool Test()
    {
        Point a(2.0f, 0.0f, 12.0f);
        Point b(0.0f, 2.0f, 12.f);
        Point c(-2.0f, 0.0f, 12.f);
        Triangle triangle(a, b, c);
        triangles.push_back(triangle);
        return true;
    }

    void SetWorldMatrix(Mat4x4<float>& worldMat)
    {
        this->worldMat = worldMat;
    }

    void SetPos(const vector3f& pos)
    {
        worldMat[3][0] = pos[0];
        worldMat[3][1] = pos[1];
        worldMat[3][2] = pos[2];
    }

    Mat4x4<float> GetWorldMatrix()
    {
        return this->worldMat;
    }

    void SetCullingState(CullingState cullingState)
    {
        this->pso.cullingState = cullingState;
    }

    void SetFillMode(FillMode fillMode)
    {
        this->pso.fillMode = fillMode;
    }

    Pso GetPso()
    {
        return pso;
    }

    void Render(std::shared_ptr<BackBuffer> backBuffer, Mat4x4<float> &vp)
    {
        auto mvp = worldMat * vp;
        backBuffer->SetPso(pso);
        for (auto &tri : triangles)
        {
            backBuffer->Rasterize(tri, mvp);
        }
    }
private:
    Mat4x4<float> worldMat;
    std::vector<Triangle> triangles;
    // pipeline state
    Pso pso;
};

class Camera
{
public:
    Camera() {}
    Mat4x4<float> GetMat()
    {
        return viewMat;
    }
    void SetPos(vector3f pos)
    {
    }
    void LookAt(const vector3f &eyePos, const vector3f &focusPos, const vector3f upDir)
    {
        auto w = focusPos - eyePos;
        auto j = vector3f{ 0.0f, 1.0f, 0.0f };
        auto u = j.Cross(w) / j.Cross(w).Length();
        auto v = w.Cross(u);
        this->viewMat = {
            u.x(), v.x(), w.x(), 0.0f,
            u.y(), v.y(), w.y(), 0.0f,
            u.z(), v.z(), w.z(), 0.0f,
            -eyePos * u, -eyePos * v, -eyePos * w, 1.0f
        };
    }
private:
    Mat4x4<float> viewMat;
};
class Scene
{
public:
    Scene(int rt_r, int rt_c) {
        colorBuffer.reset();
        colorBuffer = std::make_shared<BackBuffer>(rt_r, rt_c);
    }

    void AddMesh(std::shared_ptr<Mesh> &mesh)
    {
        meshes.push_back(mesh);
    }

    void Render()
    {
        auto vp = camera->GetMat() * projMat;
        colorBuffer->Clear();
        for (auto& mesh : meshes)
        {
            mesh->Render(colorBuffer, vp);
        }
    }

    std::shared_ptr<BackBuffer> GetBackBuffer()
    {
        return colorBuffer;
    }

    void SetViewMat(Mat4x4<float> &viewMat)
    {
        this->viewMat = viewMat;
    }

    void SetProjMat(Mat4x4<float> &projMat)
    {
        this->projMat = projMat;
    }

    void SetCamera(std::shared_ptr<Camera> &ptr)
    {
        camera = ptr;
    }

    void SaveTexture()
    {

    }

    void SetProjection(float nearPlane, float farPlane, float fovAngleY, float aspect)
    {
        // fovAngleY为弧度
        this->projMat = Mat4x4<float>(
            1.0f / aspect / tan(fovAngleY / 2), 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f / tan(fovAngleY / 2), 0.0f, 0.0f,
            0.0f, 0.0f, farPlane / (farPlane - nearPlane), 1.0f,
            0.0f, 0.0f, -farPlane * nearPlane / (farPlane - nearPlane), 0.0f
        );
    }
private:
    std::shared_ptr<BackBuffer> colorBuffer;
    std::vector<std::shared_ptr<Mesh>> meshes;
    Mat4x4<float> viewMat, projMat;
    std::shared_ptr<Camera> camera;
};
