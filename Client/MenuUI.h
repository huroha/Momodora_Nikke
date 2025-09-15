#pragma once
#include "EditorUI.h"


class MenuUI :
    public EditorUI
{
private:

public:
    virtual void Render() override;
    virtual void Render_Update() override {};

private:
    void File();
    void Level();
    void GameObject();
    void Editor();
    void Asset();

    wstring GetNextMaterialName();

    void CreateObject_LayerIdx(DWORD_PTR _ListUI, DWORD_PTR _SelectString);
    void CreatePrefab_LayerIdx(DWORD_PTR _ListUI, DWORD_PTR _SelectString);
    wstring OpenFileDialog(const wchar_t* filter);

public:
    MenuUI();
    ~MenuUI();
};

