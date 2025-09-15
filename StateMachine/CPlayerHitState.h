#pragma once
#include <Engine/CState.h>

class CPlayerHitState :
    public CState
{
private:
    bool    m_HitDirRight;
    bool    m_StartHit;
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;



public:
    CLONE(CPlayerHitState);
    CPlayerHitState();
    ~CPlayerHitState();
};

