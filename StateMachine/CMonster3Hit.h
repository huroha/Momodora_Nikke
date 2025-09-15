#pragma once
#include <Engine/CState.h>

class CMonster3Script;

class CMonster3Hit :
    public CState
{
private:
    CMonster3Script*        m_Monster3Script;
    float                   m_HitTime;
    bool                    m_HitStart;

public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CMonster3Hit);
    CMonster3Hit();
    ~CMonster3Hit();
};

