#pragma once
#include <Engine/CScript.h>

class CBoss1HpControll :
    public CScript
{
private:
    Vec3        m_OriginPos;
    Vec3        m_OriginScale;
    float m_MaxHP;         // �ִ� HP �� (�⺻�� 100)
    float m_CurrentHP;     // ���� HP ��

public:
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;

    void SettingHp(int _Hp);

public:
    CLONE(CBoss1HpControll);
    CBoss1HpControll();
    ~CBoss1HpControll();
};

