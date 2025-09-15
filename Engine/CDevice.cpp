#include "pch.h"
#include "CDevice.h"

#include "CConstBuffer.h"
#include "CAssetMgr.h"

CDevice::CDevice()
    : m_hWnd(nullptr)
    , m_arrCB{}
    , m_RSState{}
    , m_BSState{}
    , m_Sampler{}
{

}

CDevice::~CDevice()
{
    for (UINT i = 0; i < (UINT)CB_TYPE::END; ++i)
    {
        DELETE(m_arrCB[i]);
    }

}

int CDevice::Init(HWND _hWnd, Vec2 _Resolution)
{
    m_hWnd = _hWnd;
    m_Resolution = _Resolution;

    UINT iFlag = 0;

#ifdef _DEBUG
    iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
    
    D3D_FEATURE_LEVEL Level = D3D_FEATURE_LEVEL_11_0;

    // Device, Context ����
    if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
                                , iFlag, nullptr, 0, D3D11_SDK_VERSION
                                , m_Device.GetAddressOf(), &Level, m_Context.GetAddressOf())))
    {
        return E_FAIL;
    }

    // SwapChain ����
    if (FAILED(CreateSwapChain()))
    {
        return E_FAIL;
    }

    // RenderTarget, DepthStencilTex, View ����
    if (FAILED(CreateView()))
    {
        return E_FAIL;
    }

    // RasterizerState �����ϱ�
    if (FAILED(CreateRasterizerState()))
    {
        return E_FAIL;
    }

    // SamplerState �����ϱ�
    if (FAILED(CreateSamplerState()))
    {
        return E_FAIL;
    }

    // BlendState �����ϱ�
    if (FAILED(CreateBlendState()))
    {
        return E_FAIL;
    }

    // DepthStencilState �����ϱ�
    if (FAILED(CreateDepthStencilState()))
    {
        return E_FAIL;
    }

    // ����Ʈ ���� ���� -> RenderMgr �� RenderStart�� ���鶧

    // ������� ����
    if (CreateConstBuffer())
    {
        return E_FAIL;
    }

    // �����ػ� ���� ����
    g_Data.RenderResolution = m_Resolution;

    return S_OK;
}

int CDevice::CreateSwapChain()
{
    // SwapChain �� ����� ���� ����ü -> ����ü�� ������ ��Ƽ� �̷��� �����޶�� �����ϴ� �뵵
    DXGI_SWAP_CHAIN_DESC Desc = {};

    Desc.BufferCount = 1;                               // ����� ����
    Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Swapchain ����� �ؽ����� �뵵 ����

    Desc.BufferDesc.Width = m_Resolution.x;             // ����� �ػ� Width
    Desc.BufferDesc.Height = m_Resolution.y;            // ����� �ػ� Height
    Desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    Desc.BufferDesc.RefreshRate.Denominator = 1;        // ȭ�� ���� �ӵ�, �и�
    Desc.BufferDesc.RefreshRate.Numerator = 60;         // ȭ�� ���� �ӵ�, ����

    Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    Desc.OutputWindow = m_hWnd;                         // ����ۿ� �׷��� �̹����� ��½�ų ������
    Desc.Windowed = true;                               // â���, ��üȭ�� ���

    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;

    Desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;         // ������ �׷��� �̹����� �������� ����
    Desc.Flags = 0;


    ComPtr<IDXGIDevice>    IDXGIDevice = nullptr;
    ComPtr<IDXGIAdapter>   pAdapter = nullptr;
    ComPtr<IDXGIFactory>   pFactory = nullptr;

    // 128 ��Ʈ
    // GUID (Global Unique ID)
    m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)IDXGIDevice.GetAddressOf());
    IDXGIDevice->GetAdapter(pAdapter.GetAddressOf());
    pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

    if (FAILED(pFactory->CreateSwapChain(m_Device.Get(), &Desc, m_SwapChain.GetAddressOf())))
    {
        return E_FAIL;
    }

    return S_OK;
}

int CDevice::CreateView()
{
    // 1. RenderTarget �ؽ���
    //  - SwapChain �� �����ɶ� ������� ID3D11Texture2D ��ü�� ����Ų��.
    ComPtr<ID3D11Texture2D> pTex2D = nullptr;
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pTex2D.GetAddressOf());
    CAssetMgr::GetInst()->CreateTexture(L"RenderTargetTex", pTex2D);

    // ����Ÿ�ٰ� depthstencil �ؽ��ĸ� ��������� ����ִ��� ������. -> Imgui�߰� ���� �ñ�

    // 2. DepthStencil �ؽ���
    //m_DSTex = CAssetMgr::GetInst()->CreateTexture(L"DepthStencilTex", m_Resolution.x, m_Resolution.y, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);
    CAssetMgr::GetInst()->CreateTexture(L"DepthStencilTex", m_Resolution.x, m_Resolution.y, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);
    


    // Texture ������ AssetMgr���� �����ϵ��� �Ű���


    return S_OK;
}

int CDevice::CreateConstBuffer()
{
    m_arrCB[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer(CB_TYPE::TRANSFORM);
    m_arrCB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(TransInfo));

    m_arrCB[(UINT)CB_TYPE::MATERIAL] = new CConstBuffer(CB_TYPE::MATERIAL);
    m_arrCB[(UINT)CB_TYPE::MATERIAL]->Create(sizeof(MtrlConst));

    m_arrCB[(UINT)CB_TYPE::SPRITE] = new CConstBuffer(CB_TYPE::SPRITE);
    m_arrCB[(UINT)CB_TYPE::SPRITE]->Create(sizeof(SpriteInfo));

    m_arrCB[(UINT)CB_TYPE::GLOBAL] = new CConstBuffer(CB_TYPE::GLOBAL);
    m_arrCB[(UINT)CB_TYPE::GLOBAL]->Create(sizeof(GlobalData));
    return S_OK;
}

int CDevice::CreateRasterizerState()
{
    // CULL_BACK
    m_RSState[(UINT)RS_TYPE::CULL_BACK] = nullptr;

    // CULL_FRONT
    D3D11_RASTERIZER_DESC Desc = {};
    Desc.CullMode = D3D11_CULL_FRONT;
    Desc.FillMode = D3D11_FILL_SOLID;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());

    // CULL_NONE
    Desc.CullMode = D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_SOLID;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());

    // WIRE_FRAME
    Desc.CullMode = D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_WIREFRAME;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());



    return S_OK;
}

int CDevice::CreateBlendState()
{
    m_BSState[(UINT)BS_TYPE::DEFAULT] = nullptr;


    D3D11_BLEND_DESC Desc = {};

    Desc.AlphaToCoverageEnable = false;
    Desc.IndependentBlendEnable = false;
    Desc.RenderTarget[0].BlendEnable = true;
    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;       // ���󳢸� ���ϱ�
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;    // Source : PixelShader return ���� ���� ���
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;// Destination : ������ ��µ� ����Ÿ��

    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    if (FAILED(DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ALPHABLEND].GetAddressOf())))
    {
        return E_FAIL;
    }


    return S_OK;
}

int CDevice::CreateDepthStencilState()
{
    // Less
    m_DSState[(UINT)DS_TYPE::LESS] = nullptr;

    // LessEqual
    D3D11_DEPTH_STENCIL_DESC Desc = {};

    Desc.StencilEnable = false;
    Desc.DepthEnable = true;                            // �������� ����
    Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;       // �۰ų� ������� ���
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // ���� ���

    DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());

    // Greater
    Desc.StencilEnable = false;
    Desc.DepthEnable = true;                            // �������� ����
    Desc.DepthFunc = D3D11_COMPARISON_GREATER;          // ���̰� ū ��� ���
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // ���� ���

    DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::GREATER].GetAddressOf());

    // NoTest
    Desc.StencilEnable = false;
    Desc.DepthEnable = false;                           // �������� �������� ����
    Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;           // �׻� ���
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // ���� ���

    DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST].GetAddressOf());

    // NoWrite
    Desc.StencilEnable = false;
    Desc.DepthEnable = true;                            // �������� ����
    Desc.DepthFunc = D3D11_COMPARISON_LESS;             // ������� ���
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;  // ���� ������� ����

    DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());

    // NoTest NoWrite
    Desc.StencilEnable = false;
    Desc.DepthEnable = false;                           // �������� X
    Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;           // �׻� ���
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;  // ���� ������� ����

    DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());

    return S_OK;
}


int CDevice::CreateSamplerState()
{
    D3D11_SAMPLER_DESC Desc = {};

    // �̹漺 ���͸� ���÷�
    Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.Filter = D3D11_FILTER_ANISOTROPIC;
    DEVICE->CreateSamplerState(&Desc, m_Sampler[0].GetAddressOf());

    // MIN_MAG_POINT ����
    Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
    DEVICE->CreateSamplerState(&Desc, m_Sampler[1].GetAddressOf());

    CONTEXT->VSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->HSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->DSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->GSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->PSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->CSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());

    CONTEXT->VSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->HSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->DSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->GSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->PSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->CSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());



    return S_OK;
}





void CDevice::Present()
{
    // ȭ��(������) �� RenderTarget �� �׷��� �̹����� ��½�Ų��.
    m_SwapChain->Present(0, 0);
}