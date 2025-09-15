#pragma once
#include "CComponent.h"

class CCamera :
    public CComponent
{
private:
    // ����
    PROJ_TYPE   m_ProjType;     // ���� ���
    float       m_Far;          // �þ� �ִ� �Ÿ�
    float       m_AspectRatio;  // ���� ������ ���� ��Ⱦ��    ���� / ����
    UINT        m_LayerCheck;   // ī�޶� ���� ���̾� ��Ʈ üũ
    int         m_Priority;     // ī�޶� �켱���� 0 : Main Camera, -1 = �̵��

    // ��������(Perspective)
    float       m_FOV;          // FOV(Field Of View) �þ߰�

    // ��������(Orthographic)
    float       m_Width;        // �������� ���α���
    float       m_Scale;        // �������� ����

    // ��ȯ���
    Matrix      m_matView;      // View ���
    Matrix      m_matProj;      // Projection ���


    // ��ü �з�
    vector<CGameObject*>    m_vecOpaque;
    vector<CGameObject*>    m_vecMask;
    vector<CGameObject*>    m_vecTransparent;
    vector<CGameObject*>    m_vecParticle;
    vector<CGameObject*>    m_vecPostprocess;

public:
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }
    void SetWidth(float _Width) { m_Width = _Width; }
    void SetAspectRatio(float _AR) { m_AspectRatio = _AR; }
    void SetScale(float _Scale) { m_Scale = _Scale; }
    void SetFar(float _Far) { m_Far = _Far; }
    void SetFOV(float _FOV) { m_FOV = _FOV; }

    void SetPriority(int _Priority) { m_Priority = _Priority; }

public:
    float       GetScale() { return m_Scale; }
    PROJ_TYPE   GetProjType() { return m_ProjType; }
    float GetFar() { return m_Far; }
    float GetFOV() { return m_FOV; }
    float GetWidth() { return m_Width; }
    float GetAspectRatio() { return m_AspectRatio; }
    int GetCameraPriority() { return m_Priority; }
    UINT GetLayerCheck() { return m_LayerCheck; }


public:
    void LayerCheck(int _LayerIdx);
    void LayerCheckAll() { m_LayerCheck = 0xffffffff; }
    void LayerCheckClear() { m_LayerCheck = 0; }




public:

    virtual void Begin() override;
    virtual void FinalTick() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

    void SortObject();
    void Render();

public:
    CLONE(CCamera);
    CCamera();
    CCamera(const CCamera& _Origin);
    ~CCamera();
};
