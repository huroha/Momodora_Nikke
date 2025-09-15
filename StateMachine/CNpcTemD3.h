#pragma once
#include <Engine/CState.h>

class CNpcTemD3 :
    public CState
{
private:
    CGameObject* m_DialogBox;

    int             m_Dialog3;
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CNpcTemD3);
    CNpcTemD3();
    ~CNpcTemD3();
};