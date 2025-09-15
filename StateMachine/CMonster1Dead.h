#pragma once
#include <Engine/CState.h>

class CMonster1Dead :
    public CState
{
private:
    bool        m_DeadStart;
    float       m_PaperBurnInst;

    bool        m_IsDead;

public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CMonster1Dead);
    CMonster1Dead();
    ~CMonster1Dead();
};

