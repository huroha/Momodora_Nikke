#pragma once
#include <Engine/CState.h>

class CMonster1Script;

class CMonsterHit :
    public CState
{
private:
    CMonster1Script*        m_Monster1Script;
    float                   m_HitTime;
    bool                    m_HitStart;
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CMonsterHit);
    CMonsterHit();
    ~CMonsterHit();
};

