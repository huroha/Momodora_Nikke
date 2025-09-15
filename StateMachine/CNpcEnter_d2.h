#pragma once
#include <Engine/CState.h>

class CNpcEnter_d2 :
    public CState
{
    CGameObject* m_MsgBox;

    int             m_Dialog1;

public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CNpcEnter_d2);
    CNpcEnter_d2();
    ~CNpcEnter_d2();
};

