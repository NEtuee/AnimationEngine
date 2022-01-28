#include "D3Device.h"
#include "SimpleColor.h"
#include <iostream>

D3Device::D3Device()
{
}

D3Device::D3Device(const D3Device& device)
{
}

D3Device::~D3Device()
{
}

bool D3Device::initialize(int width, int height, HWND hWnd, bool vsync, bool fullscreen, float screenDepth, float screenNear)
{
    _vsync = vsync;

    //dx 그래픽 인터페이스 팩토리 생성
    IDXGIFactory* factory = nullptr;
    if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))
    {
        return false;
    }

    //그래픽 카드 인터페이스 어뎁터 생성
    IDXGIAdapter* adapter = nullptr;
    if (FAILED(factory->EnumAdapters(0, &adapter)))
    {
        return false;
    }

    //출력 어뎁터 지정
    IDXGIOutput* adapterOutput = nullptr;
    if (FAILED(adapter->EnumOutputs(0, &adapterOutput)))
    {
        return false;
    }

    //출력에서 표시 형식에 맞는 모드 수 가져오기
    unsigned int modes = 0;
    if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
        &modes, NULL)))
    {
        return false;
    }

    //가능한 모니터, 그래픽 카드 조합 저장용 리스트 생성
    DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[modes];
    if (displayModeList == nullptr)
    {
        return false;
    }

    //디스플레이 모드 리스트 채우기
    if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
        &modes, displayModeList)))
    {
        return false;
    }

    //화면 너비, 높이 맞는 디스플레이 모드 찾기
    unsigned int numerator = 0;
    unsigned int denominator = 0;
    for (unsigned int i = 0; i < modes; ++i)
    {
        if (displayModeList[i].Width == static_cast<unsigned int>(width) &&
            displayModeList[i].Height == static_cast<unsigned int>(height))
        {
            numerator = displayModeList[i].RefreshRate.Numerator;
            denominator = displayModeList[i].RefreshRate.Denominator;
        }
    }

    //그래픽 카드 구조체 얻기
    DXGI_ADAPTER_DESC adapterDesc;
    if (FAILED(adapter->GetDesc(&adapterDesc)))
    {
        return false;
    }

    //비디오 메모리 메가바이트로 저장
    _videoMemory = static_cast<int>(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

    //그래픽 카드 이름 저장
    size_t stringLength = 0;
    if (wcstombs_s(&stringLength, _videoDescription, 128, adapterDesc.Description, 128) != 0)
    {
        return false;
    }

    std::cout << _videoDescription << " : " << _videoMemory << " MB" << "\n";

    delete[] displayModeList;
    displayModeList = 0;

    adapterOutput->Release();
    adapterOutput = 0;

    adapter->Release();
    adapter = 0;

    factory->Release();
    factory = 0;

//스왑 체인

    //스왑 체인 초기화
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

    //백버퍼 하나만
    swapChainDesc.BufferCount = 1;

    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;

    //32bit 서페이스 설정
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    //백버퍼 리프레쉬 비율 설정
    if (_vsync)
    {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
    }
    else
    {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    }

    //백버퍼 사용 용도 설정
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    //윈도우 핸들 지정
    swapChainDesc.OutputWindow = hWnd;

    //멀티 샘플링 끄기
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    //전체 화면 설정
    swapChainDesc.Windowed = !fullscreen;

    //스캔 라인 순서, 크기 지정 안함
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    //백버퍼 지우기 설정
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    //추가 옵션 플래그 없음
    swapChainDesc.Flags = 0;

    //dx11로 설정
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

    //스왑체인, 디바이스, 디바이스 컨텍스트 생성
    if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
        D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, &_device, NULL, &_deviceContext)))
    {
        return false;
    }

//백버퍼

    //백버퍼 포인터 얻어오기
    ID3D11Texture2D* backBufferPointer = nullptr;
    if (FAILED(_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPointer)))
    {
        return false;
    }

    //백버퍼 포인터로 렌더 타겟 뷰 설정
    if (FAILED(_device->CreateRenderTargetView(backBufferPointer, NULL, &_renderTargetView)))
    {
        return false;
    }

    backBufferPointer->Release();
    backBufferPointer = 0;

    //뎁스 버퍼 구조체 초기화
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    //뎁스 버퍼 구조체 설정
    depthBufferDesc.Width = width;
    depthBufferDesc.Height = height;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    //뎁스 버퍼 텍스쳐 생성
    if (FAILED(_device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer)))
    {
        return false;
    }

    //스텐실 상태 구조체 초기화
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    //스텐실 상태 구조체 설정
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xff;
    depthStencilDesc.StencilWriteMask = 0xff;

    //정면 픽셀 스텐실 설정
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//후면 픽셀 스텐실 설정
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    //뎁스 스텐실 상태 생성
    if (FAILED(_device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState)))
    {
        return false;
    }

    //텝스 스텐실 상태 설정
    _deviceContext->OMSetDepthStencilState(_depthStencilState, 1);

    //뎁스 스텐실 뷰 초기화
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    //뎁스 스텐실 뷰 구조체 설정
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    //뎁스 스텐실 뷰 생성
    if (FAILED(_device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView)))
    {
        return false;
    }

    //렌더링 타겟 뷰, 깊이 스텐실 버퍼를 출력 렌더 파이프라인에 바인딩
    _deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

    //래스터라이저 구조체 설정
    D3D11_RASTERIZER_DESC rasterDesc;
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;//D3D11_FILL_WIREFRAME;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    //래스터라이저 스테이트 생성
    if (FAILED(_device->CreateRasterizerState(&rasterDesc, &_rasterizerState)))
    {
        return false;
    }

    //래스터라이저 스테이트 설정
    _deviceContext->RSSetState(_rasterizerState);

    //뷰포드 구조체 설정
    D3D11_VIEWPORT viewPort;
    viewPort.Width = static_cast<float>(width);
    viewPort.Height = static_cast<float>(height);
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 1.0f;
    viewPort.TopLeftX = 0.0f;
    viewPort.TopLeftY = 0.0f;

    //뷰포트 생성
    _deviceContext->RSSetViewports(1, &viewPort);

    float fov = 3.141592654f / 4.0f;
    float screenAspect = static_cast<float>(width) / static_cast<float>(height);

    _projectionMatrix = XMMatrixPerspectiveFovLH(fov, screenAspect, screenNear, screenDepth);
    _worldMatrix = XMMatrixIdentity();
    _orthoMatrix = XMMatrixOrthographicLH(static_cast<float>(width), static_cast<float>(height),
        screenNear, screenDepth);

    return true;
}

void D3Device::destroyDevice()
{
    if (_swapChain)
    {
        _swapChain->SetFullscreenState(false, NULL);
    }

    if (_rasterizerState)
    {
        _rasterizerState->Release();
        _rasterizerState = 0;
    }

	if (_depthStencilView)
	{
        _depthStencilView->Release();
        _depthStencilView = 0;
	}

	if (_depthStencilState)
	{
        _depthStencilState->Release();
        _depthStencilState = 0;
	}

	if (_depthStencilBuffer)
	{
        _depthStencilBuffer->Release();
        _depthStencilBuffer = 0;
	}

	if (_renderTargetView)
	{
        _renderTargetView->Release();
        _renderTargetView = 0;
	}

	if (_deviceContext)
	{
        _deviceContext->Release();
        _deviceContext = 0;
	}

	if (_device)
	{
        _device->Release();
        _device = 0;
	}

	if (_swapChain)
	{
        _swapChain->Release();
        _swapChain = 0;
	}
}

void D3Device::beginScene(const SimpleColor& color)
{
    float colors[4] = { color._r,color._g ,color._b ,color._a };
    _deviceContext->ClearRenderTargetView(_renderTargetView, colors);

    _deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3Device::endScene()
{
    //백버퍼 표시
    if (_vsync)
    {
        //새로 고침 비율 고정
        _swapChain->Present(1, 0);
    }
    else
    {
        //가능한 빨리
        _swapChain->Present(0, 0);
    }
}

ID3D11Device* D3Device::getDevice()
{
    return _device;
}

ID3D11DeviceContext* D3Device::getDeviceContext()
{
    return _deviceContext;
}

void D3Device::getProjectionMatrix(XMMATRIX& matrix)
{
    matrix = _projectionMatrix;
}

void D3Device::getWorldMatrix(XMMATRIX& matrix)
{
    matrix = _worldMatrix;
}

void D3Device::getOrthoMatrix(XMMATRIX& matrix)
{
    matrix = _orthoMatrix;
}

void D3Device::getVideoCardInfo(char* description, int& memory)
{
    strcpy_s(description, 128, _videoDescription);
    memory = _videoMemory;
}
