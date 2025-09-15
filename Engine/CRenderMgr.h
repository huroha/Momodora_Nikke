#pragma once
#include "singleton.h"
#include "CTexture.h"

class CCamera;
class CLight2D;
class CStructuredBuffer;

class CRenderMgr :
    public singleton<CRenderMgr>

{
    SINGLE(CRenderMgr)
private:

    vector<CCamera*>        m_vecCam;       // ���� ������ ������ ���� �ȿ��ִ� ī�޶��
    CCamera*                m_EditorCam;    // ������ ������ ���� �ܺ� ī�޶�

    vector<CLight2D*>       m_vecLight2D;
    CStructuredBuffer*      m_Light2DBuffer;

    list<tDebugShapeInfo>   m_DbgList;     // ����� ������ ����
    class CGameObject*      m_DbgObj;       // ����� ���� ������ ������Ʈ

    Ptr<CTexture>           m_PostProcessTex;  // ��ó�� �ؽ���(����Ÿ�� �ػ󵵿� ����)

    bool                    m_IsEditor; // �����͸�� �� or �ΰ��� ��
    bool                    m_DebugRender;

    // Fade ���� ������
    CGameObject*      m_FadeObj;
    bool              m_bFading;
    bool              m_bFadeIn;       // true: FadeIn, false: FadeOut
    bool              m_bFadeInOut;    // FadeInOut ����������
    bool              m_bFadeInDone;   // FadeIn �Ϸ� ����
    float             m_fFadeTime;     // �� Fade�� �ɸ��� �ð�
    float             m_fCurrentTime;
    float             m_fAlpha;
    float             m_fStayTime;     // FadeIn�� FadeOut ���� ���ð�
    bool              m_bFadeOutStart;  // FadeOut�� ��� ���۵Ǿ�����

public:
    void StartFadeInOut(float _fadeTime, float _stayTime = 0.f);
    bool IsFading() { return m_bFading; }
    bool IsFadeOutStart() { return m_bFadeOutStart; }


private:
    void UpdateFade();
    void RenderFade();



public:
    void RegisterCamera(CCamera* _Cam, UINT _Priority);
    void RegisterEditorCamera(CCamera* _EditorCam) { m_EditorCam = _EditorCam; }
    void DeregisterCamera() { m_vecCam.clear(); }

    void RegisterLight2D(CLight2D* _Light2D) { m_vecLight2D.push_back(_Light2D); }
    void AddDebugShape(const tDebugShapeInfo& _info) { m_DbgList.push_back(_info); }
    void CopyRenderTarget();
    void SetEditorMode(bool _IsEditor) { m_IsEditor = _IsEditor; }

public:
    void Init();
    void Render();

private:

    void RenderStart();
    void Binding();
    void Render_Debug();

    void Render_Play();
    void Render_Editor();

};

