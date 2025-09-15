#pragma once
#include <Engine/CState.h>

class CNpcEnter_d3 :
    public CState
{
private:
    CGameObject* m_MsgBox;

    int             m_Dialog3;
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CNpcEnter_d3);
    CNpcEnter_d3();
    ~CNpcEnter_d3();
};

