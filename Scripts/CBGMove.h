#pragma once

#include <Engine/CScript.h>

class CBGMove :
    public CScript
{
private:
    CGameObject*    m_Target;
    int             m_Idx;
    Vec3            m_StartTargetPos;       // 타겟의 시작 위치
    Vec3            m_StartBackgroundPos;   // 배경의 시작 위치
public:

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;

    float Lerp(float a, float b, float t) { return a + (b - a) * t; }

    float SmoothStep(float a, float b, float t)
    {
        float normalized = std::clamp(t, 0.f, 1.f);
        float factor = normalized * normalized * (3.f - 2.f * normalized);
        return a + (b - a) * factor;
    }

    void SetTargetObject(CGameObject* _Obj) { m_Target = _Obj; }


public:
    CLONE(CBGMove);
    CBGMove();
    ~CBGMove();
};

