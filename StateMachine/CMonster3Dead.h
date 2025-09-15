#pragma once
#include <Engine/CState.h>

class CMonster3Dead :
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
    CLONE(CMonster3Dead);
    CMonster3Dead();
    ~CMonster3Dead();
};

