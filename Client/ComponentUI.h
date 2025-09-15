#pragma once
#include "EditorUI.h"

#include <Engine/CGameObject.h>

class ComponentUI :
    public EditorUI
{
private:
    CGameObject* m_TargetObject;
    const COMPONENT_TYPE  m_Type;

public:
    void SetTargetObject(CGameObject* _Target);
    CGameObject* GetTargetObject() { return m_TargetObject; }
    COMPONENT_TYPE GetType() { return m_Type; }


protected:
    void ComponentTitle(const string& _title,Vec3 _Color = Vec3(0.f,0.6f,0.6f));
    void ComponentSubtitle(const string& _title, Vec3 _Color = Vec3(0.7f,0.9f,0.f));

public:
    ComponentUI(const string& _ID, COMPONENT_TYPE _Type);
    ~ComponentUI();
};

