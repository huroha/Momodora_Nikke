#include "pch.h"
#include "CPauseFont.h"

#include <Engine/CFontMgr.h>
#include <Engine/CTimeMgr.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CSoundMgr.h>

CPauseFont::CPauseFont()
    : CScript((UINT)SCRIPT_TYPE::PAUSEFONT)
    , m_ActiveCheck(false)
    , m_BgmVolumeId(-1)
    , m_FxVolumeId(-1)
{
}

CPauseFont::~CPauseFont()
{
}



void CPauseFont::SaveComponent(FILE* _File)
{
}

void CPauseFont::LoadComponent(FILE* _File)
{
}

void CPauseFont::Begin()
{

    CreateMainFont();
    CreateOptionFont();
    CreateMemoFont();

    StateMachine()->ChangeState(L"CPauseMain");




}

void CPauseFont::Tick()
{
    if (StateMachine()->GetCurStateName() == L"CPauseOption")
    {
        if (!m_ActiveCheck)
        {
            CFontMgr::GetInst()->SetPauseActive(m_vecPauseFont[3], false);
            CFontMgr::GetInst()->SetPauseActive(m_vecPauseFont[4], false);
            CTimeMgr::GetInst()->FontActive(false);

            for (int i = 0; i < m_OptionId.size(); ++i)
            {
                CFontMgr::GetInst()->SetPauseActive(m_OptionId[i], true);
            }
            m_ActiveCheck = true;
        }
    }
    else if (StateMachine()->GetCurStateName() == L"CPauseMain")
    {
        if (!m_ActiveCheck)
        {
            CFontMgr::GetInst()->SetPauseActive(m_vecPauseFont[3], true);
            CFontMgr::GetInst()->SetPauseActive(m_vecPauseFont[4], true);
            // Time과 Slay Count active 관련이다.
            CTimeMgr::GetInst()->FontActive(true);

            for (int i = 0; i < m_OptionId.size(); ++i)
            {
                CFontMgr::GetInst()->SetPauseActive(m_OptionId[i], false);
            }
            m_ActiveCheck = true;
        }
    }
    else
    {
        if (!m_ActiveCheck)
        {
            CFontMgr::GetInst()->SetPauseActive(m_vecPauseFont[3], false);
            CFontMgr::GetInst()->SetPauseActive(m_vecPauseFont[4], false);
            CTimeMgr::GetInst()->FontActive(false);
            

            m_ActiveCheck = true;
        }


    }
}

void CPauseFont::CreateMainFont()
{

    int FontID = 0;
    wstring Dialog = L"메모";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 124.f, 46.f, 36.f, FONT_RGBA(255, 255, 255, 255));
    m_vecPauseFont.push_back(FontID);
    Dialog = L"모험 일지";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 560.f, 46.f, 36.f, FONT_RGBA(255, 255, 255, 255));
    m_vecPauseFont.push_back(FontID);
    Dialog = L"옵션";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 1080.f, 46.f, 36.f, FONT_RGBA(255, 255, 255, 255));
    m_vecPauseFont.push_back(FontID);

    Dialog = L"플레이 시간 : ";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 504.f, 252.f, 24.f, FONT_RGBA(255, 247, 215, 255));
    m_vecPauseFont.push_back(FontID);

    Dialog = L"처치한 적 : ";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 504.f, 306.f, 24.f, FONT_RGBA(255, 247, 215, 255));
    m_vecPauseFont.push_back(FontID);

    Dialog = L"메뉴 변경";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 88.f, 718.f, 22.f, FONT_RGBA(255, 255, 255, 255));
    m_vecPauseFont.push_back(FontID);

    Dialog = L"선택";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 280.f, 718.f, 22.f, FONT_RGBA(255, 255, 255, 255));
    m_vecPauseFont.push_back(FontID);

    Dialog = L"게임 재개";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 400.f, 718.f, 22.f, FONT_RGBA(255, 255, 255, 255));
    m_vecPauseFont.push_back(FontID);


}

void CPauseFont::CreateOptionFont()
{
    int FontID = 0;
    wstring Dialog;
    Dialog = L"BGM 음량 : ";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 300.f, 26.f, FONT_RGBA(255, 247, 215, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_OptionId.push_back(FontID);
    Dialog = L"효과음 음량 :";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 340.f, 26.f, FONT_RGBA(255, 247, 215, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_OptionId.push_back(FontID);

    // 이 ID는 따로 넘길 필요가 있음.
    int BgmVol = CSoundMgr::GetInst()->GetBGMVolume() * 100;
    Dialog = to_wstring(BgmVol);
    m_BgmVolumeId = CFontMgr::GetInst()->RegisterPauseText(Dialog, 760.f, 300.f, 26.f, FONT_RGBA(255, 247, 215, 255));
    CFontMgr::GetInst()->SetPauseActive(m_BgmVolumeId, false);
    m_OptionId.push_back(m_BgmVolumeId);

    int FxVol = CSoundMgr::GetInst()->GetFXVolume() * 100;
    Dialog = to_wstring(FxVol);
    m_FxVolumeId = CFontMgr::GetInst()->RegisterPauseText(Dialog, 760.f, 340.f, 26.f, FONT_RGBA(255, 247, 215, 255));
    CFontMgr::GetInst()->SetPauseActive(m_FxVolumeId, false);
    m_OptionId.push_back(m_FxVolumeId);

}

void CPauseFont::CreateMemoFont()
{
    int FontID = 0;
    wstring Dialog;

    float temp = 50.f;
    Dialog = L"체력, 마력";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 196.f, 244.f, 30.f, FONT_RGBA(100, 100, 100, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 244.f, 30.f, FONT_RGBA(250, 237, 130, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    Dialog = L"화면 우측 상단의 초록색 게이지는 체력으로,\n체력을 수치화한 숫자 역시 표시됩니다.체력이 0이 되면\n전투에서 패배하며 종의 사당으로 돌아갑니다.\n\n마력은 체력 게이지 아래에 있는 파란색\n게이지로 표시됩니다.";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 286.f, 26.f, FONT_RGBA(255, 247, 215, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);


    Dialog = L"점프";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 196.f, 244.f + temp, 30.f, FONT_RGBA(100, 100, 100, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 244.f, 30.f, FONT_RGBA(250, 237, 130, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    Dialog = L"사용 키 : Space Bar\n\n점프는 발판 위로 뛰어오르는 기본 동작입니다.";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 286.f, 26.f, FONT_RGBA(255, 247, 215, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    Dialog = L"근접 공격";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 196.f, 244.f+ temp*2, 30.f, FONT_RGBA(100, 100, 100, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 244.f, 30.f, FONT_RGBA(250, 237, 130, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    Dialog = L"사용 키 : A\n\n플레이어가 바라보는 방향으로 공격합니다.\n버튼을 연달아 누르면 콤보를 사용합니다.";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 286.f, 26.f, FONT_RGBA(255, 247, 215, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    Dialog = L"이동과 회피";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 196.f, 244.f+ temp*3, 30.f, FONT_RGBA(100, 100, 100, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 244.f, 30.f, FONT_RGBA(250, 237, 130, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    Dialog = L"사용 키 : 화살표 & X\n\n좌우 이동과 구르기를 통한 회피를 할 수 있습니다.\n구르기는 공중이 아닌 어느상황에서도 사용할 수 있습니다.\n\n몬스터와 공격을 피해없이 통과가 가능합니다.";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 286.f, 26.f, FONT_RGBA(255, 247, 215, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    Dialog = L"특수 스킬 : 버스트";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 196.f, 244.f + temp * 4, 30.f, FONT_RGBA(100, 100, 100, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 244.f, 30.f, FONT_RGBA(250, 237, 130, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    Dialog = L"상호작용 키 : R\n\n마나를 100 소모하여 10초간 버스트 상태가 됩니다.\n기본 공격이 강화되며 마지막 공격은\n전체공격으로 강화됩니다.\n\n마나는 몬스터 타격시 회복 할 수 있습니다.";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 286.f, 26.f, FONT_RGBA(255, 247, 215, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);


    Dialog = L"종의 사당";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 196.f, 244.f+ temp*5, 30.f, FONT_RGBA(100, 100, 100, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 244.f, 30.f, FONT_RGBA(250, 237, 130, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    Dialog = L"상호작용 키 : 공격\n\n신성한 종을 울려 생명력과 마력을 완전히\n회복합니다. 전투에서 패배 시 항상 마지막으로\n울린 신성한 종으로 돌아갑니다.";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 286.f, 26.f, FONT_RGBA(255, 247, 215, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);


}





