#pragma once
#include <Engine/CScript.h>

class CPlayButtonScript :
    public CScript
{
private:
    int     m_StartDialog;

    wstring m_NextLevelPath;

    bool    m_IsFadeOutChecked;
    bool    m_None;
public:
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;

public:
    CLONE(CPlayButtonScript);
    CPlayButtonScript();
    ~CPlayButtonScript();
};

