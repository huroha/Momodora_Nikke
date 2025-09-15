#pragma once
#include <Engine/CState.h>

class CBoss1Attack3 :
    public CState
{
private:
    bool        m_Once;
    bool        m_BossDirRight;
    bool        m_JumpOnce;
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CBoss1Attack3);
    CBoss1Attack3();
    ~CBoss1Attack3();
};

