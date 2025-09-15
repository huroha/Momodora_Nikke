#pragma once
#include <Engine/CState.h>
class CPlayerJumpUp :
    public CState
{
private:

public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CPlayerJumpUp);
    CPlayerJumpUp();
    ~CPlayerJumpUp();

};

