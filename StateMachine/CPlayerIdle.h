#pragma once


#include <Engine/CState.h>

class CPlayerIdle :
    public CState
{
private:

public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CPlayerIdle);
    CPlayerIdle();
    ~CPlayerIdle();
};

