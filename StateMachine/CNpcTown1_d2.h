#pragma once
#include <Engine/CState.h>

// √π ¥Î»≠

class CNpcTown1_d2 :
    public CState
{
private:
    CGameObject*    m_MsgBox;

    int             m_Dialog1;
    int             m_Dialog2;
    bool            m_Next;

public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CNpcTown1_d2);
    CNpcTown1_d2();
    ~CNpcTown1_d2();
};
