#pragma once
#include "DirectXDefine.h"

class Shader : public AlignedAllocationPolicy<16>
{
public:
					Shader();
					Shader(const Shader&);
					~Shader();

	bool			initialize(ID3D11Device* device, HWND hWnd);
	void			destroyShader();
	bool			render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX projection);
private:
	bool			initializeShader(ID3D11Device* device, HWND hWnd, WCHAR* vertexShader, WCHAR* pixelShader);
	void			destroyShaderReal();
	void			outputShaderErrorMessage(ID3D10Blob* blob, HWND hWnd, WCHAR* message);

	bool			setShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection);
	void			renderShader(ID3D11DeviceContext* deviceContext, int indexCount);
private:
	ID3D11VertexShader*		_vertexShader = nullptr;
	ID3D11PixelShader*		_pixelShader = nullptr;
	ID3D11InputLayout*		_inputLayout = nullptr;
	ID3D11Buffer*			_matrixBuffer = nullptr;
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
};

