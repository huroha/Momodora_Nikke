#pragma once
#include <Engine/CScript.h>

class CPlayerScript;

class CHpControll :
    public CScript
{
private:
    Vec3        m_FullHpPos;
    Vec3        m_FullHpScale;
    float m_MaxHP;         // �ִ� HP �� (�⺻�� 100)
    float m_CurrentHP;     // ���� HP ��

public:
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;

    void SettingHp(int _Hp);

public:
    CLONE(CHpControll);
    CHpControll();
    ~CHpControll();
};

