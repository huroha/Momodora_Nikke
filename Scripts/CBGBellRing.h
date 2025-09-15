#pragma once
#include <Engine/CScript.h>

class CBGBellRing :
    public CScript
{
    
private:
    Ptr<CPrefab>    m_Prefab;
    vector<CGameObject*> m_vecObj;       // 0. PostObj 1. BellObj 2. RestoreTex 3. HpTex 4. MpTex


    bool            m_IsCollision;
    float           m_Time;

public:

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

public:
    CLONE(CBGBellRing);
    CBGBellRing();
    ~CBGBellRing();
};

