#pragma once
#include <Engine/CState.h>

class CBoss1Idle2 :
    public CState
{
private:
    bool    m_IdleStart;
    float   m_AtkCoolTime;
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CBoss1Idle2);
    CBoss1Idle2();
    ~CBoss1Idle2();
};

