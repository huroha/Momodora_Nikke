#pragma once
#include <Engine/CState.h>

class CNpcEnter_d1 :
    public CState
{
    CGameObject* m_MsgBox;

public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CNpcEnter_d1);
    CNpcEnter_d1();
    ~CNpcEnter_d1();
};

