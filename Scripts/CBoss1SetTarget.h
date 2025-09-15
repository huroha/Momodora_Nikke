#pragma once
#include    <Engine/CScript.h>

class CBoss1SetTarget :
    public CScript
{
private:

    bool        m_LookOn;
    bool        m_Once;
public:

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    bool    GetLookon() { return m_LookOn; }


public:
    CLONE(CBoss1SetTarget);
    CBoss1SetTarget();
    ~CBoss1SetTarget();
};

