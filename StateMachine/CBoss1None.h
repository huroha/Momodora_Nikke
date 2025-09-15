#pragma once
#include <Engine/CState.h>

class CBoss1None :
    public CState
{
private:
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CBoss1None);
    CBoss1None();
    ~CBoss1None();
};

