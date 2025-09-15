#pragma once

#include <Engine/CState.h>

class CMonster2Attack :
    public CState
{
private:
    bool        m_DirRight;
    bool        m_Jump;
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CMonster2Attack);
    CMonster2Attack();
    ~CMonster2Attack();
};

