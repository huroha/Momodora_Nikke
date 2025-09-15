#pragma once
#include "ComponentUI.h"
class FlipbookPlayerUI :
    public ComponentUI
{
public:
    virtual void Render_Update() override;


private:
    void AddFlipbook_List(DWORD_PTR _ListUI, DWORD_PTR _SelectString);

public:
    FlipbookPlayerUI();
    ~FlipbookPlayerUI();

};

