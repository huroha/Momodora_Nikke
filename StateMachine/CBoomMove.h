#pragma once
#include <Engine/CState.h>

class CBoomMove :
    public CState
{
private:
    bool    m_IsRight;
    bool    m_RotateStart;

    int     m_Power;
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CBoomMove);
    CBoomMove();
    ~CBoomMove();
};

