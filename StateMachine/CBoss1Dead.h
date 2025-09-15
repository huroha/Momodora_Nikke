#pragma once
#include <Engine/CState.h>

class CBoss1Dead :
    public CState
{
private:
    bool        m_DeadStart;
    float       m_PaperBurnInst;

    bool        m_IsDead;

    CGameObject* m_BossBar;
    CGameObject* m_BossName;
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CBoss1Dead);
    CBoss1Dead();
    ~CBoss1Dead();
};

