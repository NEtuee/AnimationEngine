#include "Shader.h"
#include <iostream>
Shader::Shader()
{
}

Shader::Shader(const Shader&)
{
}

Shader::~Shader()
{
}

bool Shader::initialize(ID3D11Device* device, HWND hWnd)
{
    WCHAR vs[] = L"Color.vs";
    WCHAR ps[] = L"Color.ps";
    return initializeShader(device,hWnd,vs,ps);
}

void Shader::destroyShader()
{
    destroyShaderReal();
}

bool Shader::render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX projection)
{
    if (setShaderParameters(deviceContext, world, view, projection) == false)
    {
        return false;
    }

    renderShader(deviceContext, indexCount);
    return true;
}

bool Shader::initializeShader(ID3D11Device* device, HWND hWnd, WCHAR* vertexShader, WCHAR* pixelShader)
{
    ID3D10Blob* errorMessage = nullptr;

    ID3D10Blob* vertexShaderBuffer = nullptr;
    if (FAILED(D3DCompileFromFile(vertexShader, NULL, NULL, "VSMain", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
        0, &vertexShaderBuffer, &errorMessage)))
    {
        if (errorMessage)
        {
            outputShaderErrorMessage(errorMessage, hWnd, vertexShader);
        }
        else
        {
            MessageBox(hWnd, vertexShader, L"Missing Shader File", MB_OK);
        }

        return false;
    }

	ID3D10Blob* pixelShaderBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(pixelShader, NULL, NULL, "PSMain", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		0, &pixelShaderBuffer, &errorMessage)))
	{
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hWnd, pixelShader);
		}
		else
		{
			MessageBox(hWnd, pixelShader, L"Missing Shader File", MB_OK);
		}

		return false;
	}

    if (FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader)))
    {
        return false;
    }

	if (FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader)))
	{
		return false;
	}

    D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

    unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    if (FAILED(device->CreateInputLayout(polygonLayout, numElements,
        vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_inputLayout)))
    {
        return false;
    }

    vertexShaderBuffer->Release();
    vertexShaderBuffer = 0;

    pixelShaderBuffer->Release();
    pixelShaderBuffer = 0;

    D3D11_BUFFER_DESC matrixBufferDesc;
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    if (FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer)))
    {
        return false;
    }
    
    return true;
}

void Shader::destroyShaderReal()
{
    if (_matrixBuffer)
    {
        _matrixBuffer->Release();
        _matrixBuffer = 0;
    }

    if (_inputLayout)
    {
        _inputLayout->Release();
        _inputLayout = 0;
    }

    if (_pixelShader)
    {
        _pixelShader->Release();
        _pixelShader = 0;
    }

    if (_vertexShader)
    {
        _vertexShader->Release();
        _vertexShader = 0;
    }
}

void Shader::outputShaderErrorMessage(ID3D10Blob* blob, HWND hWnd, WCHAR* message)
{
    std::cout << (reinterpret_cast<const char*>(blob->GetBufferPointer()));

    blob->Release();
    blob = 0;

    MessageBox(hWnd, L"Error compiling shader", message, MB_OK);
}

bool Shader::setShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection)
{
    //쉐이더용 행렬 만들기
    world = XMMatrixTranspose(world);
    view = XMMatrixTranspose(view);
    projection = XMMatrixTranspose(projection);

    //상수 버퍼에 내용 쓰려고 잠그기
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    if (FAILED(deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
    {
        return false;
    }

    //상수 버퍼 데이터 포인터 가져오기
    MatrixBufferType* dataPointer = (MatrixBufferType*)mappedResource.pData;

    //행렬 복사
    dataPointer->world = world;
    dataPointer->view = view;
    dataPointer->projection = projection;

    //잠금 해제
    deviceContext->Unmap(_matrixBuffer, 0);

    //버텍스 쉐이더 상수 버퍼 위치
    unsigned int bufferNumber = 0;

    //버텍스 쉐이더 상수 버퍼 교체
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);

    return true;
}

void Shader::renderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
    deviceContext->IASetInputLayout(_inputLayout);

    deviceContext->VSSetShader(_vertexShader, NULL, 0);
    deviceContext->PSSetShader(_pixelShader, NULL, 0);

    deviceContext->DrawIndexed(indexCount,0,0);
}
