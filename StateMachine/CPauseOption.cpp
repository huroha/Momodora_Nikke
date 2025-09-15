#include "pch.h"
#include "CPauseOption.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CSoundMgr.h>
#include <Engine/CFontMgr.h>
#include <Scripts/CPauseFont.h>



CPauseOption::CPauseOption()
    : m_OptionTrigger(false)
    , m_OptionBgm(false)
    , m_BgmID(-1)
    , m_FxID(-1)
{
}

CPauseOption::~CPauseOption()
{
}

void CPauseOption::FinalTick()
{
    if (!CLevelMgr::GetInst()->IsPauseState())
        return;

    // 모험일지로 넘어감
    if (!m_OptionTrigger && KEY_TAP(KEY::LEFT))
    {
        GetStateMachine()->ChangeState(L"CPauseMain");
    }
    // 메모로 넘어감
    else if (!m_OptionTrigger && KEY_TAP(KEY::RIGHT))
    {

    }

    if (m_OptionTrigger && KEY_TAP(KEY::UP))
    {
        m_vecObj[1]->Transform()->SetRelativePos(Vec3(-24.f, 67.f, 100.f));
        m_OptionBgm = true;
    }
    else if (m_OptionTrigger && KEY_TAP(KEY::DOWN))
    {
        m_vecObj[1]->Transform()->SetRelativePos(Vec3(-24.f, 28.f, 100.f));
        m_OptionBgm = false;
    }

    // 음량 조절 설정
    if (KEY_TAP(KEY::SPACE))
    {
        m_OptionTrigger = !m_OptionTrigger;
        m_vecObj[1]->GetRenderComponent()->SetActive(m_OptionTrigger);
    }

    if (m_OptionTrigger && CLevelMgr::GetInst()->IsPauseState())
    {
        // 음량 조절 여기서하자
        // Bgm 음량 조절

        if (m_OptionBgm && KEY_TAP(KEY::LEFT))
        {
            float BgmVolume = CSoundMgr::GetInst()->GetBGMVolume();
            int intVolume = static_cast<int>(BgmVolume * 10);
            if (BgmVolume > 0)
            {
                intVolume -= 1;
                BgmVolume = static_cast<float>(intVolume) / 10.f;
            }
            CSoundMgr::GetInst()->SetBGMVolume(BgmVolume);
            wstring Volume = to_wstring((intVolume * 10));
            CFontMgr::GetInst()->UpdatePauseText(m_BgmID, Volume);
        }
        else if(m_OptionBgm && KEY_TAP(KEY::RIGHT))
        {
            float BgmVolume = CSoundMgr::GetInst()->GetBGMVolume();
            int intVolume = static_cast<int>(BgmVolume * 10);
            if (BgmVolume < 1.f)
            {
                intVolume +=1;
                BgmVolume = static_cast<float>(intVolume) / 10.f;
            }
            CSoundMgr::GetInst()->SetBGMVolume(BgmVolume);
            wstring Volume = to_wstring((intVolume * 10));
            CFontMgr::GetInst()->UpdatePauseText(m_BgmID, Volume);
        }

        // Fx 음량 조절
        if (!m_OptionBgm && KEY_TAP(KEY::LEFT))
        {
            float FxVolume = CSoundMgr::GetInst()->GetFXVolume();
            int intVolume = static_cast<int>(FxVolume * 10);
            if (FxVolume > 0)
            {
                intVolume -= 1;
                FxVolume = static_cast<float>(intVolume) / 10.f;
            }
            CSoundMgr::GetInst()->SetFXVolume(FxVolume);
            wstring Volume = to_wstring((intVolume * 10));
            CFontMgr::GetInst()->UpdatePauseText(m_FxID, Volume);
        }
        else if (!m_OptionBgm && KEY_TAP(KEY::RIGHT))
        {
            float FxVolume = CSoundMgr::GetInst()->GetFXVolume();
            int intVolume = static_cast<int>(FxVolume * 10);
            if (FxVolume < 1.f)
            {
                intVolume += 1;
                FxVolume = static_cast<float>(intVolume) / 10.f;
            }
            CSoundMgr::GetInst()->SetFXVolume(FxVolume);
            wstring Volume = to_wstring((intVolume * 10));
            CFontMgr::GetInst()->UpdatePauseText(m_FxID, Volume);
        }
    
    }

}

void CPauseOption::Enter()
{
    CGameObject* pObj;
    pObj = CLevelMgr::GetInst()->FindObjectByName(L"Point_Pause");
    m_vecObj.push_back(pObj);
    pObj = CLevelMgr::GetInst()->FindObjectByName(L"Point_Pause2");
    m_vecObj.push_back(pObj);

    m_vecObj[0]->Transform()->SetRelativePos(Vec3(480.f, 316.f, 100.f));
    m_vecObj[1]->GetRenderComponent()->SetActive(false);
    m_vecObj[1]->Transform()->SetRelativePos(Vec3(-24.f, 67.f, 100.f));
    pObj = CLevelMgr::GetInst()->FindObjectByName(L"BG_Pasue");
   
    CPauseFont* pBgTarget = (CPauseFont*)pObj->GetScripts()[0];
    pBgTarget->ResetActiveCheck();
    m_BgmID = pBgTarget->GetBgmVolumeID();
    m_FxID = pBgTarget->GetFxVolumeID();

    m_OptionBgm = true;

}

void CPauseOption::Exit()
{

    m_vecObj[1]->GetRenderComponent()->SetActive(false);
    m_vecObj.clear();
}