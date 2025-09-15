#pragma once
#include <Engine/CState.h>


class CPlayerAttack3 :
    public CState
{
private:
    bool        m_NextAttack;
    bool        m_PlayerDirRight;
    bool        m_IsBust;
    vector<CGameObject*>    m_Child;
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;




public:
    CLONE(CPlayerAttack3);
    CPlayerAttack3();
    ~CPlayerAttack3();
};

