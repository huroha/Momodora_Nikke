#pragma once
#include "EditorUI.h"

class TreeNode;
class CGameObject;


class Outliner :
    public EditorUI
{
private:
    class TreeUI* m_Tree;

public:
    virtual void Render_Update() override;

    void RenewGameObject();
private:
    void AddGameObject(TreeNode* _ParentNode, CGameObject* _Object);

    void SelectGameObject(DWORD_PTR _TreeNode);
    void DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode);

public:
    Outliner();
    ~Outliner();
};

