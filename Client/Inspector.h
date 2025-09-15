#pragma once
#include "EditorUI.h"


#include <Engine/Ptr.h>
#include <Engine/assets.h>

class CGameObject;
class ComponentUI;
class ScriptUI;
class AssetUI;
class AddComponentUI;

class Inspector :
    public EditorUI
{
private:
    CGameObject* m_TargetObject;
    ComponentUI* m_arrComUI[(UINT)COMPONENT_TYPE::END];
    AddComponentUI* m_AddComponentUI;

    vector<ScriptUI*>   m_vecScriptUI;

    Ptr<CAsset>         m_TargetAsset;
    AssetUI* m_arrAssetUI[(UINT)ASSET_TYPE::END];

public:
    void SetTargetObject(CGameObject* _Target);
    void SetTargetAsset(Ptr<CAsset> _Asset);
    CGameObject* GetTargetObject() { return m_TargetObject; }

    void DeleteComponentCheck(COMPONENT_TYPE _Type);
    void ReLoad();

public:
    virtual void Render_Update() override;
    virtual void Tick() override;

private:
    void CreateComponentUI();
    void CreateAssetUI();
public:
    Inspector();
    ~Inspector();
};

