#pragma once
#include <Engine/CScript.h>
class CHpReduceFx :
    public CScript
{
private:
    Vec3        m_FullHpPos;
    Vec3        m_FullHpScale;
    float       m_MaxHP;         // �ִ� HP �� (�⺻�� 100)
    float       m_CurrentHP;     // ���� HP ��

    bool        m_Trigger;
    float       m_DistHp;

public:
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;
    Vec3 Lerp(const Vec3& start, const Vec3& end, float t);
    void SettingHp(int _Hp);

public:
    CLONE(CHpReduceFx);
    CHpReduceFx();
    ~CHpReduceFx();

};

