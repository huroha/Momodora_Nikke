#pragma once
#include    <Engine/CScript.h>

class CBoss1Script :
    public CScript
{
private:
    CGameObject*    m_Target;
    vector<CGameObject*>    m_vecObj;

    Ptr<CPrefab>    m_PostPref;
    Ptr<CPrefab>    m_ParticlePref;
    Ptr<CPrefab>    m_DustParticlePref;

    

    int             m_AtkCount;
    bool            m_RoarAttackOn;
    bool            m_Once;
    float           m_PrefTime;

    bool            m_BressOn;
    bool            m_BressOnce;
    float           m_BressTime;

    int             m_HP;

    bool            m_DustOn;
    float           m_DustTime;

    bool            m_DeadOnce;
public:

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;


    void SetTargetObject(CGameObject* _Obj) { m_Target = _Obj; }

    int GetMonsterHP() { return m_HP; }

    void    HitDamage(int _DMG)
    {
        m_HP -= _DMG;
        if (m_HP < 0)
            m_HP = 0;
    }


    // Attack ฐüทร
public:
    void RoarAtkOn() { m_RoarAttackOn = true; }
    int  GetAttackCount() { return m_AtkCount; }
    void IncreaseAtkCount() { ++m_AtkCount; }
    void ResetAtkCount() { m_AtkCount = 0; }

    void IsBressOn() { m_BressOn = true; }


public:
    CLONE(CBoss1Script);
    CBoss1Script();
    ~CBoss1Script();
};

