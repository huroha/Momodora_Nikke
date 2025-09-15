#pragma once
#include <Engine/CState.h>

class CPauseOption :
    public CState
{
private:
    vector<CGameObject*>    m_vecObj;
    bool                    m_OptionTrigger;    // 음량 조절로 넘어올건지
    bool                    m_OptionBgm;      // BGM 바꿀건지 Fx 바꿀건지
    int                     m_BgmID;            
    int                     m_FxID;
    
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CPauseOption);
    CPauseOption();
    ~CPauseOption();

};

