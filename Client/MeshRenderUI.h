#pragma once
#include "ComponentUI.h"

class MeshRenderUI :
    public ComponentUI
{
private:


public:
    virtual void Render_Update() override;

private:
    void SelectMesh(DWORD_PTR _ListUI, DWORD_PTR _SelectString);
    void SelectMaterial(DWORD_PTR _ListUI, DWORD_PTR _SelectString);

public:
    MeshRenderUI();
    ~MeshRenderUI();
};
