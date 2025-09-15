#pragma once
#include <Engine/CState.h>

class CNpcTemD1 :
    public CState
{
private:

public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CNpcTemD1);
    CNpcTemD1();
    ~CNpcTemD1();
};

