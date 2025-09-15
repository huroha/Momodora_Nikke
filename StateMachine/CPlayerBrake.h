#pragma once
#include <Engine/CState.h>
class CPlayerBrake :
    public CState
{
private:
    float   m_SlopeAngle;
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CPlayerBrake);
    CPlayerBrake();
    ~CPlayerBrake();
};

