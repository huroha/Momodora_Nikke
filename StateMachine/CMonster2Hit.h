#pragma once
#include <Engine\CState.h>



class CMonster2Script;


class CMonster2Hit :
    public CState
{
private:
    CMonster2Script* m_Monster2Script;
    float                   m_HitTime;
    bool                    m_HitStart;
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CMonster2Hit);
    CMonster2Hit();
    ~CMonster2Hit();

};

