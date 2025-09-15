#pragma once
#include "CEntity.h"

enum SB_TYPE
{
    SRV_ONLY,
    SRV_UAV,
};

class CStructuredBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>                m_SB_Main;
    ComPtr<ID3D11Buffer>                m_SB_Write;
    ComPtr<ID3D11Buffer>                m_SB_Read;


    ComPtr<ID3D11ShaderResourceView>    m_SRV;
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;

    D3D11_BUFFER_DESC                   m_Desc;

    SB_TYPE                             m_Type;

    UINT                                m_ElementCount; // ��� ����
    UINT                                m_ElementSize;  // ��Ҵ� ũ��

    bool                                m_SysMemMove;   // SB_Write �� Read �� ���ų�? UAV�Ⱦ��°͵��� �� �̰� Ŵ

public:
    UINT GetElementSize() { return m_ElementSize; }
    UINT GetElementCount() { return m_ElementCount; }
    UINT GetBufferSize() { return m_ElementSize * m_ElementCount; }

    void SetData(void* _SystemMem, UINT _ElementCount = 0);
    void GetData(void* _SysDest);

    // Rendering, t Binding
    void Binding(UINT _RegisterNum);

    // Rendering, t clear
    void Clear(UINT _RegisterNum);

    // ComputeShader, t Bindng
    void Binding_CS_SRV(UINT _RegisterNum);
    // ComputeShader, u Binding
    void Binding_CS_UAV(UINT _RegisterNum);

    // ComputeShader, t Clear
    void Clear_CS_SRV(UINT _RegisterNum);
    // ComputeShader, U Clear
    void Clear_CS_UAV(UINT _RegisterNum);


public:
    int Create(UINT _ElementSize, UINT _ElementCount, SB_TYPE _Type, bool _SystemMemMove, void* _SysMem = nullptr);

public:
    CLONE_DISABLE(CStructuredBuffer);
    CStructuredBuffer();
    ~CStructuredBuffer();
};

