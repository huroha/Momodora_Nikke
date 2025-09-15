#include "pch.h"
#include "CBossStageScript.h"

#include <Engine/CRenderMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CFontMgr.h>

#include "CPlayerScript.h"
#include <Engine/CSoundMgr.h>

CBossStageScript::CBossStageScript()
	: CScript((UINT)SCRIPT_TYPE::BOSSSTAGESCRIPT)
	, m_IsFadeOutCheck(false)
	, m_ColliderCheck(false)
{
}

CBossStageScript::~CBossStageScript()
{
}


void CBossStageScript::SaveComponent(FILE* _File)
{
}

void CBossStageScript::LoadComponent(FILE* _File)
{
}

void CBossStageScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	m_ColliderCheck = true;
	CRenderMgr::GetInst()->StartFadeInOut(1.f);
	CGameObject* pPlayer = CLevelMgr::GetInst()->FindObjectByName(L"Player");
	CPlayerScript* Script = (CPlayerScript*)pPlayer->GetScripts()[0];
	int HpID = Script->GetFontHp_Id();
	int MpID = Script->GetFontMp_Id();

	CFontMgr::GetInst()->StartFadeOut(HpID, 1.f);
	CFontMgr::GetInst()->StartFadeOut(MpID, 1.f);

	CSoundMgr::GetInst()->FadeOut(L"Temple_Bgm", 0.5f);
	CSoundMgr::GetInst()->FadeIn(L"Boss_Bgm", 4.f,1.f);

}

void CBossStageScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBossStageScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBossStageScript::Begin()
{
	m_NextLevelPath2 = CPathMgr::GetInst()->GetContentPath() + L"Level\\Stage_Boss.lv";

}

void CBossStageScript::Tick()
{

	if (!m_IsFadeOutCheck && CRenderMgr::GetInst()->IsFadeOutStart() && m_ColliderCheck)
	{
		m_IsFadeOutCheck = true;
		ChangeLevel(CLevelMgr::g_LevelLoad(m_NextLevelPath2), LEVEL_STATE::PLAY);

	}
}
