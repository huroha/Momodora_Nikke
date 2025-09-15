#pragma once
#include <Engine/CState.h>
class CBombBoom :
    public CState
{
private:


public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CBombBoom);
    CBombBoom();
    ~CBombBoom();
};

