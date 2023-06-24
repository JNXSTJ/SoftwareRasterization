// soft_ras.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
// 坐标系， 左手坐标系, 和dx一致
// a, b, c, (b - a) x (c - b) 为法线方向

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <opencv2/opencv.hpp>
#include "MatUtil.hpp"
#include "world.hpp"

using namespace DirectX;
using namespace DirectX::PackedVector;

bool test = true;

std::ostream& operator<<(std::ostream& os, XMMATRIX& obj) {
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
            float value = DirectX::XMVectorGetByIndex(obj.r[i], j);
			os << std::setw(10) << std::setprecision(4) << std::fixed << value << " ";
		}
		os << std::endl;
	}
	return os;
}

void Test()
{
    Mat4x4<float> mat;
    vector3f eyePos{ -20, 35, -50 };
    vector3f focusPos{ 10, 0, 30 };
    vector3f up{ 0, 1, 0};

    mat.LookAt(eyePos, focusPos, up);
    std::cout << mat << std::endl;
    
    auto eyePos1 = XMVectorSet(-20, 35, -50, 1.0f);
    auto focusPos1 = XMVectorSet(10, 0, 30, 1.0f);
    auto up1 = XMVectorSet(0, 1, 0, 0.0f);
    auto ret = XMMatrixLookAtLH(eyePos1, focusPos1, up1);
    std::cout << ret << std::endl;
}

void Test2()
{

    auto ret =Mat4x4<float>::SetProjection(1.0f, 100.0f, XMConvertToRadians(45.0), 4.0f / 3.0f);
    std::cout << ret << std::endl;
    auto ret2 = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0), 4.0f / 3.0f, 1.0f, 100.0f);
    std::cout << ret2 << std::endl;
}


int main()
{
    const int width = 700, height = 700;
    std::shared_ptr<Mesh> skull = std::make_shared<Mesh>();
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(width, height);
    std::string path = "skull.txt";
    if (test)
    {
        skull->Test();
    }
    else
    {
		auto ret = skull->Load(path);
		if (ret == false)
		{
			printf("load skull failed!\n");
			exit(1);
		}
    }
    scene->AddMesh(skull);

    constexpr double PI = 3.141592653;
    float nearPlane = 0.1f, farPlane = 50.0f;
    float fovAngleY = PI / 4.0f, aspect = 1.0f;
    scene->SetProjection(nearPlane, farPlane, fovAngleY, aspect);

    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    camera->LookAt({0, 0, 5}, {0, 0, 6}, {0, 1, 0});
    scene->SetCamera(camera);
    std::shared_ptr<BackBuffer> backBuffer = std::make_shared<BackBuffer>();

    int key = 0;
    int frame_count = 0;
    while (key != 27)
    {
        scene->Render();

        auto data = scene->GetBackBuffer()->Data();
        cv::Mat image(width, height, CV_32FC4, data.data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';
    }

    std::cout << "Hello World!\n";
}

