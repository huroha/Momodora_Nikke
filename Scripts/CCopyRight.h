#pragma once
#include <Engine/CScript.h>
class CCopyRight :
    public CScript
{
private:
    vector<int> m_FontId;

    bool        m_Trigger;
    bool        m_IsFadeOutChecked;
public:
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;

public:
    CLONE(CCopyRight);
    CCopyRight();
    ~CCopyRight();
};

