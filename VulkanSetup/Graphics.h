#pragma once
#include <windows.h>
#include <vector>
#include "DirectXDefine.h"

const bool fullScreen = false;
const bool vsyncEnabled = false;
const float screenDepth = 1000.f;
const float screenNear = 0.1f;

class D3Device;
class Camera;
class Model;
class Shader;
class Scene;

class Graphics
{
public:
					Graphics();
					~Graphics();

	Model*			createCube(XMFLOAT4 color);
	Model*			createQuad(float size, int width, int height);
	Model*			createRectangle();
	Shader*			createBaseShader();

	D3Device*		getDevice() { return _directDevice; }

	void			renderBegin(Model* model);
	bool			renderModelToShader(Shader* shader, Model* model, XMMATRIX world, Camera* camera);

	bool			initialize(int width, int height, HWND hWnd);
	void			destroyGraphics();

	bool			frame();
	bool			render(std::vector<Scene*>*, Camera*);
private:
	D3Device*		_directDevice = nullptr;
	HWND			_hWnd;
};

