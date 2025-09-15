#pragma once

#include <Engine/CScript.h>

class CMissileScript :
    public CScript
{
private:
    Vec3        m_Velocity;

public:
    void SetVelocity(Vec3 _Velocity) { m_Velocity = _Velocity; }
    Vec3 GetVelocity() { return m_Velocity; }

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);


    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

public:
    CLONE(CMissileScript);
    CMissileScript();
    ~CMissileScript();
};
