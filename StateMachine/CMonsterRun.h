#pragma once
#include <Engine/CState.h>

class CMonsterRun :
    public CState
{
private:
    vector<CGameObject*> m_vecChild;
    bool                    m_IsRight;
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CMonsterRun);
    CMonsterRun();
    ~CMonsterRun();
};

