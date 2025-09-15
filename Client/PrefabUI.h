#pragma once
#include "AssetUI.h"
class PrefabUI :
    public AssetUI
{
private:

public:
    virtual void Render_Update() override;


    void SelectLayerIdx(DWORD_PTR _ListUI, DWORD_PTR _SelectString);
public:
    PrefabUI();
    ~PrefabUI();
};

