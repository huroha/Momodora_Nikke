#pragma once
#include "CEntity.h"
#include "CStateMachine.h"
#include "CComponent.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CAssetMgr.h"
#include "components.h"

class CState :
    public CEntity
{
private:
    CStateMachine*          m_Owner;
    UINT                    m_StateType;
public:
    CStateMachine* GetStateMachine() { return m_Owner; }


public:
    virtual void Enter() = 0;
    virtual void FinalTick() = 0;
    virtual void Exit() = 0;


    UINT GetStateType() { return m_StateType; }

public:
    virtual CState* Clone() = 0;
    CState();
    ~CState();

    friend class CStateMachine;
};

