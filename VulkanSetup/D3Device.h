#pragma once
#include <windows.h>
#include "DirectXDefine.h"

struct SimpleColor;
class D3Device
{
public:
								D3Device();
								D3Device(const D3Device& device);
								~D3Device();

	bool						initialize(int width, int height, HWND hWnd, 
									bool vsync, bool fullscreen, float screenDepth, float screenNear);
	void						destroyDevice();

	void						beginScene(const SimpleColor& color);
	void						endScene();

	ID3D11Device*				getDevice();
	ID3D11DeviceContext*		getDeviceContext();

	void						getProjectionMatrix(XMMATRIX& matrix);
	void						getWorldMatrix(XMMATRIX& matrix);
	void						getOrthoMatrix(XMMATRIX& matrix);

	void						getVideoCardInfo(char* description, int& memory);

private:
	bool						_vsync					= false;
	int							_videoMemory			= 0;
	char						_videoDescription[128]	= { 0 };
	IDXGISwapChain*				_swapChain				= nullptr;
	ID3D11Device*				_device					= nullptr;
	ID3D11DeviceContext*		_deviceContext			= nullptr;
	ID3D11RenderTargetView*		_renderTargetView		= nullptr;
	ID3D11Texture2D*			_depthStencilBuffer		= nullptr;
	ID3D11DepthStencilState*	_depthStencilState		= nullptr;
	ID3D11DepthStencilView*		_depthStencilView		= nullptr;
	ID3D11RasterizerState*		_rasterizerState		= nullptr;
	XMMATRIX					_projectionMatrix;
	XMMATRIX					_worldMatrix;
	XMMATRIX					_orthoMatrix;
};

