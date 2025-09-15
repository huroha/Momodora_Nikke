#pragma once
#include "ComponentUI.h"
class ParticleSystemUI :
    public ComponentUI
{
private:



public:
    virtual void Render_Update() override;


private:
    void SelectParticleTex(DWORD_PTR _ListUI, DWORD_PTR _SelectString);

public:
    ParticleSystemUI();
    ~ParticleSystemUI();

};

