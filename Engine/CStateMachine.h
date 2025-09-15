#pragma once
#include "CComponent.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CAssetMgr.h"
#include "components.h"
class CState;

class CStateMachine :
    public CComponent
{
private:
    map<wstring, CState*>       m_mapState;
    CState*                     m_CurState;
    
public:
    void AddState(const wstring& _StateName, CState* _State);
    void AddStateLoad(const wstring& _StateName, CState* _State);
    CState* FindState(const wstring& _StateName);
    void ChangeState(const wstring& _StateName);
    void RemoveState(const wstring& _StateName);

    wstring GetCurStateName();
    map<wstring, CState*>& GetStateContainer() { return m_mapState; }
    void ChangeStateNone() { m_CurState = nullptr; }
    CState* GetCurState() { return m_CurState; }
public:
    virtual void FinalTick() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

public:
    CLONE(CStateMachine);
    CStateMachine();
    CStateMachine(const CStateMachine& _Origin);
    ~CStateMachine();

};

