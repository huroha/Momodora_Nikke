#pragma once
#include "ComponentUI.h"
class StateMachineUI :
    public ComponentUI
{
private:
    vector<string>      m_StringStorage;
    vector<const char*> m_StatePointers;

    int                 m_CurStateIdx;
public:
    virtual void Render_Update() override;


    void AddState_List(DWORD_PTR _ListUI, DWORD_PTR _SelectString);
    void RemoveState_List(DWORD_PTR _ListUI, DWORD_PTR _SelectString);
public:
    StateMachineUI();
    ~StateMachineUI();
};

