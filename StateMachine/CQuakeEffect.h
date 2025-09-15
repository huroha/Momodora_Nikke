#pragma once
#include <Engine/CState.h>

class CQuakeEffect :
    public CState
{
private:
    bool    m_CycleStart;
    bool    m_EndCheck;
    bool    m_BossDirRight;
    int     m_RepeatCount;
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CQuakeEffect);
    CQuakeEffect();
    ~CQuakeEffect();
};

