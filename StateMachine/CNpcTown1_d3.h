#pragma once
#include <Engine/CState.h>

class CNpcTown1_d3 :
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
    CLONE(CNpcTown1_d3);
    CNpcTown1_d3();
    ~CNpcTown1_d3();
};

