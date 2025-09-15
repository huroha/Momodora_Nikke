#pragma once
#include    <Engine/CScript.h>

class CPauseFont :
    public CScript
{
private:
    vector<int> m_vecPauseFont;
    vector<int> m_OptionId;
    vector<int> m_MemoId;


    bool        m_ActiveCheck;
    int         m_BgmVolumeId;
    int         m_FxVolumeId;

public:
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;

    int GetBgmVolumeID() { return m_BgmVolumeId; }
    int GetFxVolumeID() { return m_FxVolumeId; }

    void    ResetActiveCheck() { m_ActiveCheck = false; }

    vector<int>& GetMemoFont() { return m_MemoId; }

private:
    void CreateMainFont();
    void CreateOptionFont();
    void CreateMemoFont();

public:
    CLONE(CPauseFont);
    CPauseFont();
    ~CPauseFont();
};

