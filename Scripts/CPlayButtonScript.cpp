#include "pch.h"
#include "CPlayButtonScript.h"

#include <Engine/CFontMgr.h>
#include <Engine/CPathMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CSoundMgr.h>
CPlayButtonScript::CPlayButtonScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYBUTTONSCRIPT)
	, m_StartDialog(-1)
	, m_None(false)
	, m_IsFadeOutChecked(false)
{

}

CPlayButtonScript::~CPlayButtonScript()
{

}

void CPlayButtonScript::SaveComponent(FILE* _File)
{

}

void CPlayButtonScript::LoadComponent(FILE* _File)
{

}

void CPlayButtonScript::Begin()
{
	wstring Dialog = L"-Press A To START-";
	m_StartDialog = CFontMgr::GetInst()->RegisterText(Dialog, 550.f, 584.f, 24.f, FONT_RGBA(255, 255, 255, 255));

	m_NextLevelPath = CPathMgr::GetInst()->GetContentPath() + L"Level\\Temple.lv";

	CSoundMgr::GetInst()->PlayBGM(L"Lobby_Bgm", 0.5f);
}

void CPlayButtonScript::Tick()
{
	if (KEY_TAP(KEY::A) )//&& KEY_TAP(KEY::DOWN) && KEY_TAP(KEY::ENTER)
	{
		m_None = true;
		CRenderMgr::GetInst()->StartFadeInOut(1.f);
		CFontMgr::GetInst()->StartFadeOut(m_StartDialog, 1.f);
		m_IsFadeOutChecked = false;  // fadeout 체크 초기화
		CSoundMgr::GetInst()->FadeOut(L"Lobby_Bgm", 0.8f);

	}

	// fadeout 시작 체크를 한 번만 수행
	if (m_None && !m_IsFadeOutChecked && CRenderMgr::GetInst()->IsFadeOutStart())
	{
		m_IsFadeOutChecked = true;  // 체크 완료 표시
		ChangeLevel(CLevelMgr::g_LevelLoad(m_NextLevelPath), LEVEL_STATE::PLAY);


		CSoundMgr::GetInst()->FadeIn(L"Temple_Bgm", 0.8f,1.f);
	}

}
