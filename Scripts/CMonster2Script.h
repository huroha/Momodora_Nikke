#pragma once
#include    <Engine/CScript.h>

class CMonster2Script :
    public CScript
{
private:
    CGameObject*    m_Target;
    bool            m_Trace;

    bool            m_IsHit;
    float           m_Time;

    int             m_HP;
    bool            m_Dead;

public:

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    void SetTrace(bool _trace) { m_Trace = _trace; }
    bool GetTrace() { return m_Trace; }

    void SetTargetObject(CGameObject* _Obj) { m_Target = _Obj; }

    int GetMonsterHP() { return m_HP; }
    void SetHit(bool _Hit) { m_IsHit = _Hit; }
    bool GetHit() { return m_IsHit; }
    void SetDead() { m_Dead = true; }
    bool CheckDead() { return m_Dead; }
 
    void    HitDamage(int _DMG)
    {
        m_HP -= _DMG;
        if (m_HP < 0)
            m_HP = 0;
    }

public:
    CLONE(CMonster2Script);
    CMonster2Script();
    ~CMonster2Script();
};