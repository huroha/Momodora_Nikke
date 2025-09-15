#pragma once
#include <Engine/CState.h>


class CNpcTemD2 :
    public CState
{
private:
    CGameObject*    m_DialogBox;

    int             m_Dialog1;
    int             m_Dialog2;
    bool            m_Next;
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CNpcTemD2);
    CNpcTemD2();
    ~CNpcTemD2();
};