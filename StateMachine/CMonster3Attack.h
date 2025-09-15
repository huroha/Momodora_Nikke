#pragma once
#include <Engine/CState.h>

class CMonster3Attack :
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
    CLONE(CMonster3Attack);
    CMonster3Attack();
    ~CMonster3Attack();
};

