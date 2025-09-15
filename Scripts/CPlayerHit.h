#pragma once
#include    <Engine/CScript.h>


class CPlayerHit :
    public CScript
{
private:
    Ptr<CPrefab>            m_Prefab;
    vector<CGameObject*>    m_vecObj;

    bool        m_IsHit;
    bool        m_HitCool;
    float       m_CoolTime;
    float       m_HitTime;


    bool        m_Bust;
    float       m_BustTime;

    bool        m_HitDirRight;
public:

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    bool  GetHitDir() { return m_HitDirRight; }
    bool  IsBust() { return m_Bust; }

    void  UI_CalculateHPMP(int _value, bool _isHP);

public:
    CLONE(CPlayerHit);
    CPlayerHit();
    ~CPlayerHit();
};

