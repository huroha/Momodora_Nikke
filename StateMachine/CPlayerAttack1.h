#pragma once
#include <Engine/CState.h>

class CPlayerAttack1 :
    public CState
{
private:
    bool        m_NextAttack;
    bool        m_PlayerDirRight;
    bool        m_AttackSucces;

public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    void SetAttackSuccess(bool _Attack) { m_AttackSucces = _Attack; }


public:
    CLONE(CPlayerAttack1);
    CPlayerAttack1();
    ~CPlayerAttack1();
};

