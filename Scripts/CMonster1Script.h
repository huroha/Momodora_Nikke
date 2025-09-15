#pragma once
#include    <Engine/CScript.h>

class CMonster1Script :
    public CScript
{
private:
    CGameObject*    m_Target;
    bool            m_Trace;

    float           m_Time;

    int             m_HP;

public:

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

public:
    void SetTrace(bool _trace) { m_Trace = _trace; }
    bool GetTrace() { return m_Trace; }

    void SetTargetObject(CGameObject* _Obj) { m_Target = _Obj; }

    int GetMonsterHP() { return m_HP; }

    void    HitDamage(int _DMG) 
    {
        m_HP -= _DMG;
        if (m_HP < 0)
            m_HP = 0;
    }


public:
    CLONE(CMonster1Script);
    CMonster1Script();
    ~CMonster1Script();
};

