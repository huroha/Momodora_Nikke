#pragma once
#include <Engine/CState.h>

class CBoss1Attack :
    public CState
{
private:
    CGameObject*    m_AtkObj;
    CGameObject*    m_Camera;
    bool            m_CreatePref;
    bool            m_BossDirRight;

    int             m_Random;
    int             m_GetAtkCount;
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CBoss1Attack);
    CBoss1Attack();
    ~CBoss1Attack();
};

