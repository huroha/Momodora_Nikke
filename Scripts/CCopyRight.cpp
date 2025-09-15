#include "pch.h"
#include "CCopyRight.h"

#include <Engine/CFontMgr.h>
#include <Engine/CSoundMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CRenderMgr.h>

CCopyRight::CCopyRight()
    :CScript(SCRIPT_TYPE::COPYRIGHT)
    , m_Trigger(false)
    , m_IsFadeOutChecked(false)
{
}

CCopyRight::~CCopyRight()
{
}

void CCopyRight::SaveComponent(FILE* _File)
{
}

void CCopyRight::LoadComponent(FILE* _File)
{
}

void CCopyRight::Begin()
{
    wstring Dialog = L"�� ������ �¸��� ����: Nikke �� �� ���̵� ��������,\n\n    ����� �̿��� �������� ������� �����ּ���.";
    int FontID = CFontMgr::GetInst()->RegisterText(Dialog, 320.f, 220.f, 26.f, FONT_RGBA(255, 255, 255, 255));

    Dialog = L"produce by : huroha";
    FontID = CFontMgr::GetInst()->RegisterText(Dialog, 0.f, 700.f, 16.f, FONT_RGBA(255, 255, 255, 255));

    Dialog = L"������ E-mail : kmsms0306@naver.com";
    FontID = CFontMgr::GetInst()->RegisterText(Dialog, 0.f, 720.f, 16.f, FONT_RGBA(255, 255, 255, 255));

    Dialog = L"���� �� Ȯ�� : Press A KEY";
    FontID = CFontMgr::GetInst()->RegisterText(Dialog, 500.f, 580.f, 22.f, FONT_RGBA(255, 255, 255, 255));

    m_Trigger = false;
}

void CCopyRight::Tick()
{
    if (KEY_TAP(KEY::A))
    {
        if (!m_Trigger)
        {
            m_Trigger = true;
            CRenderMgr::GetInst()->StartFadeInOut(1.f);
            m_IsFadeOutChecked = false;  // fadeout üũ �ʱ�ȭ
        }

    }
    
    if (m_Trigger && !m_IsFadeOutChecked && CRenderMgr::GetInst()->IsFadeOutStart())
    {
        wstring wLevel = CPathMgr::GetInst()->GetContentPath() + L"Level\\MainStart.lv";
        ChangeLevel(CLevelMgr::g_LevelLoad(wLevel), LEVEL_STATE::PLAY);
        CSoundMgr::GetInst()->FadeIn(L"Lobby_Bgm", 0.8f, 1.f);
    }
}
