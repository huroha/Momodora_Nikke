#pragma once
#include "CAsset.h"


// 모양을 결정
// Rect Mesh/ Rect Debug/ Circle 등등

class CMesh :
    public CAsset
{
private:
    ComPtr<ID3D11Buffer>		m_VB;           // vertex buffer
    ComPtr<ID3D11Buffer>		m_IB;           // index buffer

    Vtx*                        m_VtxSysMem;
    UINT*                       m_IdxSysMem;

    D3D11_BUFFER_DESC           m_VBDesc;
    UINT                        m_VtxCount;

    D3D11_BUFFER_DESC           m_IBDesc;
    UINT                        m_IdxCount;

private:
    virtual int Load(const wstring& _FilePath) override { return S_OK; }        // Asset에서 순수가상함수처리해버렸음
    virtual int Save(const wstring& _FilePath) override { return S_OK; }

public:
    int  Create(Vtx* _pVtx, UINT _VtxCount, UINT* _pIdx, UINT _IdxCount);
    void Binding();
    void Render_Particle(int _Count);
    void Render();


public:
    CLONE_DISABLE(CMesh);
    CMesh(bool _bEngineRes = false);
    ~CMesh();
};

