#pragma once
#include "singleton.h"
#include "CTexture.h"
class CConstBuffer;

class CDevice :
    public singleton<CDevice>
{
    SINGLE(CDevice)
private:
    HWND                               m_hWnd;         // 화면을 그릴 윈도우
    Vec2                               m_Resolution;   // 렌더링 해상도
    

    ComPtr<ID3D11Device>               m_Device;       // GPU 제어, GPU 메모리에 데이터를 생성하는 역할
    ComPtr<ID3D11DeviceContext>        m_Context;      // GPU 제어, GPU 동작 명령,Rendering (그래픽 파이프라인 단계 제어), GPGPU(General Purpose GPU : 그래픽스 외의 일반적인 연산에 사용 ex) 물리 시뮬, 이미지 처리를 GPU에서 실행)

    ComPtr<IDXGISwapChain>             m_SwapChain;    // BackBuffer 관리 및 최종 렌더링 제출용

    CConstBuffer*                      m_arrCB[(UINT)CB_TYPE::END];     // 상수 버퍼를 디바이스에서 관리
    ComPtr<ID3D11RasterizerState>      m_RSState[(UINT)RS_TYPE::END];
    ComPtr<ID3D11BlendState>           m_BSState[(UINT)BS_TYPE::END];
    ComPtr<ID3D11DepthStencilState>    m_DSState[(UINT)DS_TYPE::END];


    ComPtr<ID3D11SamplerState>         m_Sampler[2];






public:
    ComPtr<ID3D11Device>            GetDevice() { return m_Device;}
    ComPtr<ID3D11DeviceContext>     GetContext() { return m_Context;}
    ComPtr<ID3D11RasterizerState>   GetRSState(RS_TYPE _Type) { return m_RSState[(UINT)_Type]; }
    ComPtr<ID3D11BlendState>        GetBSState(BS_TYPE _Type) { return m_BSState[(UINT)_Type]; }
    ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) { return m_DSState[(UINT)_Type]; }



    CConstBuffer*   GetCB(CB_TYPE _Type) { return m_arrCB[(UINT)_Type]; }

    // 클리어 날림. -> Imgui도 보여야하기때문에 client의 메인으로 뺐던거같음.
    void Present();

public:
    Vec2 GetRenderResolution() { return m_Resolution; }


public:
    int Init(HWND _hWnd, Vec2 _Resolution);

private:
    int CreateSwapChain();
    int CreateView();
    int CreateConstBuffer();
    int CreateRasterizerState();
    int CreateBlendState();
    int CreateDepthStencilState();
    int CreateSamplerState();
};

