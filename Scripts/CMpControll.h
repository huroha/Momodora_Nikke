#pragma once
#include <Engine/CScript.h>

class CMpControll :
    public CScript
{
private:
    Vec3        m_OriginPos;
    Vec3        m_OriginScale;

    float m_MaxMP;         // 최대 MP 값 (기본값 100)
    float m_CurrentMP;     // 현재 MP 값
public:
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;

    void SettingMp(int _Mp);

public:
    CLONE(CMpControll);
    CMpControll();
    ~CMpControll();
};

