#pragma once
#include "ComponentUI.h"

class Collider2DUI :
    public ComponentUI
{
private:

public:
    virtual void Render_Update() override;

public:
    Collider2DUI();
    ~Collider2DUI();
};

