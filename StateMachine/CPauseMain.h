#pragma once
#include <Engine/CState.h>

class CPauseMain :
    public CState
{
private:
    vector<CGameObject*> m_vecObj;
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CPauseMain);
    CPauseMain();
    ~CPauseMain();

};

