#pragma once
#include "EditorUI.h"
class AddComponentUI :
    public EditorUI
{
private:
    CGameObject*            m_TargetObject;

    vector<string>          m_StringStorage;
    vector<const char*>     m_ComponentPointers;
    vector<UINT>            m_vComponent;

    int                     m_ComponentIdx;

public:
    CGameObject* GetTargetObject() { return m_TargetObject; }
    void SetTargetObject(CGameObject* _Target) { m_TargetObject = _Target; }
    void AddScript(DWORD_PTR _ListUI, DWORD_PTR _SelectString);
    void CheckComponent();

    virtual void Render_Update() override;

public:
    AddComponentUI();
    ~AddComponentUI();
};

