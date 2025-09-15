#pragma once
#include <Engine/CScript.h>


class CChangeStage :
    public CScript
{
private:

    wstring      m_NextLevelPath;
    int          m_CurStageIdx;
    int          m_NextStageIdx;
    int          m_BGMTrigger;
    bool         m_ColliderCheck;
    bool         m_IsFadeOutCheck;

    Vec2         m_SpawnPos;

public:
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;


    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Begin() override;
    virtual void Tick() override;

public:
    CLONE(CChangeStage);
    CChangeStage();
    ~CChangeStage();
};

