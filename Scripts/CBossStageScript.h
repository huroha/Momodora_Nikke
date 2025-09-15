#pragma once
#include <Engine/CScript.h>

class CBossStageScript :
    public CScript
{
private:
    wstring m_NextLevelPath2;
    bool    m_ColliderCheck;
    bool    m_IsFadeOutCheck;
public:
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Begin() override;
    virtual void Tick() override;

public:
    CLONE(CBossStageScript);
    CBossStageScript();
    ~CBossStageScript();
};

