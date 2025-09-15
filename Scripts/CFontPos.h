#pragma once
#include <Engine/CScript.h>

class CFontPos :
    public CScript
{
private:
    int         m_TestID;

public:
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;

public:
    CLONE(CFontPos);
    CFontPos();
    ~CFontPos();
};

