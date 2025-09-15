#pragma once
#include <Engine/CState.h>
class CPlayerRoll :
    public CState
{
private:
    bool    m_PlayerDirRight;
    float   m_SlopeAngle;
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;


public:
    CLONE(CPlayerRoll);
    CPlayerRoll();
    ~CPlayerRoll();
};

