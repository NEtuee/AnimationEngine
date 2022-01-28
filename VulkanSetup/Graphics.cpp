#include "Graphics.h"
#include "D3Device.h"
#include "SimpleColor.h"

#include "MCube.h"
#include "MQuad.h"
#include "MRectangle.h"

#include "Shader.h"

#include "Scene.h"
#include "Camera.h"

#include "Timer.h"
#include "GUI_FPSAndCPUCounter.h"
#include "TransformStructure.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <iostream>


Graphics::Graphics()
	: _directDevice(nullptr), _hWnd(0)
{
}


Graphics::~Graphics()
{
}

Model* Graphics::createCube(XMFLOAT4 color)
{
	auto cube = new MCube;
	cube->setColor(color);
	//_model->setValue(1.f, 10, 10);
	if (cube->initialize(_directDevice->getDevice()) == false)
	{
		MessageBox(_hWnd, L"Failed to initialize model", L"Error", MB_OK);
		return nullptr;
	}

	return cube;
}

Model* Graphics::createQuad(float size, int width, int height)
{
	auto quad = new MQuad;
	quad->setValue(size, width, height);
	if (quad->initialize(_directDevice->getDevice()) == false)
	{
		MessageBox(_hWnd, L"Failed to initialize model", L"Error", MB_OK);
		return nullptr;
	}

	return quad;
}

Model* Graphics::createRectangle()
{
	auto rect = new MRectangle;
	if (rect->initialize(_directDevice->getDevice()) == false)
	{
		MessageBox(_hWnd, L"Failed to initialize model", L"Error", MB_OK);
		return nullptr;
	}

	return rect;
}

Shader* Graphics::createBaseShader()
{
	auto shader = new Shader();
	if (shader->initialize(_directDevice->getDevice(), _hWnd) == false)
	{
		MessageBox(_hWnd, L"Failed to initialize shader", L"Error", MB_OK);
		return nullptr;
	}

	return shader;
}

void Graphics::renderBegin(Model* model)
{
	model->render(_directDevice->getDeviceContext());
}

bool Graphics::renderModelToShader(Shader* shader, Model* model, XMMATRIX worldMatrix, Camera* camera)
{
	XMMATRIX viewMatrix, projectionMatrix;
	camera->getViewMatrix(viewMatrix);
	_directDevice->getProjectionMatrix(projectionMatrix);

	if (shader->render(_directDevice->getDeviceContext(),model->getIndexCount(),worldMatrix, viewMatrix, projectionMatrix) == false)
	{
		return false;
	}

	return true;
}

bool Graphics::initialize(int width, int height, HWND hWnd)
{
	_directDevice = new D3Device();
	if (_directDevice == nullptr)
	{
		return false;
	}

	//directx 초기화
	RECT rect = { 0 };
	GetClientRect(hWnd, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	if (_directDevice->initialize(width, height, hWnd, 
		vsyncEnabled, fullScreen, screenDepth, screenNear) == false)
	{
		MessageBox(hWnd, L"device initialize failed", L"Error",MB_OK);
		return false;
	}

	//imgui 초기화
	ImGui_ImplDX11_Init(hWnd, _directDevice->getDevice(), _directDevice->getDeviceContext());

	return true;
}

void Graphics::destroyGraphics()
{
	ImGui_ImplDX11_Shutdown();

	if (_directDevice)
	{
		_directDevice->destroyDevice();
		delete _directDevice;
		_directDevice = 0;
	}

}

bool Graphics::frame()
{
	return true;
}

bool Graphics::render(std::vector<Scene*>* scenes, Camera* camera)
{
	ImGui_ImplDX11_NewFrame();

	SimpleColor color(.5f,.5f,.5f,1.f);
	_directDevice->beginScene(color);

	camera->render();

	for (auto iterator = scenes->begin(); iterator != scenes->end(); ++iterator)
	{
		(*iterator)->renderScene(this, camera);
	}

	ImGui::Render();

	_directDevice->endScene();

	return true;
}
