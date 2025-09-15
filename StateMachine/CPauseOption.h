#pragma once
#include <Engine/CState.h>

class CPauseOption :
    public CState
{
private:
    vector<CGameObject*>    m_vecObj;
    bool                    m_OptionTrigger;    // ���� ������ �Ѿ�ð���
    bool                    m_OptionBgm;      // BGM �ٲܰ��� Fx �ٲܰ���
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

