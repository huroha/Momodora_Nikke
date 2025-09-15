#pragma once
#include <Engine/CState.h>

class CBoss1Attack2 :
    public CState
{
private:
    CGameObject*    m_Bress;
    CGameObject*    m_Camera;
    bool            m_BossDirRight;

    bool            m_Start;
    bool            m_IdleChange;
    bool            m_Once;
    float           m_Time;

    int             m_Random;
    int             m_GetAtkCount;
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CBoss1Attack2);
    CBoss1Attack2();
    ~CBoss1Attack2();
};

