#pragma once
#include    <Engine/CScript.h>

class CMonster1Hit :
    public CScript
{
private:
    bool        m_IsHit;
    float       m_HitTime;

public:

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);



public:
    CLONE(CMonster1Hit);
    CMonster1Hit();
    ~CMonster1Hit();

};

