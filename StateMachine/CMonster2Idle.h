#pragma once
#include <Engine/CState.h>

class CMonster2Script;

class CMonster2Idle :
    public CState
{
private:
    CMonster2Script* m_Monster2Script;
    float               m_Time;
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CMonster2Idle);
    CMonster2Idle();
    ~CMonster2Idle();
};

