#pragma once
#include <Engine/CScript.h>

class CCameraScript :
    public CScript
{
private:
    float   m_CameraSpeed;

    CGameObject* m_Target;
    wstring     m_CamName;
    float       m_OriginY;
    // Camera Oscillation
    Vec2		m_ShakeOffset;

    float		m_Frequency;
    float		m_Amplitude;
    float		m_Duration;
    float		m_AccTime;
    float		m_Dir;
    bool		m_CamShake;

    float m_DirX;  // x�� ��鸲 ����
    float m_AmplitudeX;  // x�� ��鸲 ����
    float m_FrequencyX;  // x�� ��鸲 ���ļ�

public:
    virtual void Tick() override;
    virtual void Begin() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    float Lerp(float a, float b, float t) { return a + (b - a) * t; }

    void SetTargetObject(CGameObject* _Obj) { m_Target = _Obj; }
    void CameraEffect();
    // Oscillation
    void SetCamShake(float _AmplitudeY, float _FrequencyY, float _Duration,
                     float _AmplitudeX, float _FrequencyX)
    {
        m_Frequency = _FrequencyY;
        m_Amplitude = _AmplitudeY;

        // X�� ��鸲 �Ķ���� �߰�
        m_AmplitudeX = _AmplitudeX;
        m_FrequencyX = _FrequencyX;

        m_Duration = _Duration;
        m_Dir = 1.f;
        m_DirX = 1.f;
        m_AccTime = 0.f;
        m_CamShake = true;
    }

private:
    void CameraOrthgraphicMove();
    void CameraPerspectiveMove();

public:
    CLONE(CCameraScript);
    CCameraScript();
    ~CCameraScript();
};
