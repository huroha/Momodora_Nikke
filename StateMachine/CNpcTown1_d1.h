#pragma once
#include <Engine/CState.h>

// Idle ���� == ��ȭ ���� ��
class CNpcTown1_d1 :
    public CState
{
private:
    CGameObject*    m_MsgBox;

public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CNpcTown1_d1);
    CNpcTown1_d1();
    ~CNpcTown1_d1();
};

