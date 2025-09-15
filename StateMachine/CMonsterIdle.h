#pragma once
#include <Engine/CState.h>

class CMonsterIdle :
    public CState
{
private:
    vector<CGameObject*>    m_vecChild;
    bool                    m_IsRight;

    bool                    m_Start;
    float                   m_CoolTime;

public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CMonsterIdle);
    CMonsterIdle();
    ~CMonsterIdle();
};

