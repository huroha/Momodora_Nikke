#include "pch.h"
#include "CChangeStage.h"
#include <Engine/CDataMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CSoundMgr.h>
#include <Engine/CFontMgr.h>
#include <Engine/CRenderMgr.h>
#include "CPlayerScript.h"


CChangeStage::CChangeStage()
    : CScript((UINT)SCRIPT_TYPE::CHANGESTAGE)
    , m_CurStageIdx(-1)
	, m_NextStageIdx(-1)
	, m_BGMTrigger(-1)
	, m_ColliderCheck(false)
	, m_IsFadeOutCheck(false)
{
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "CurStage Idx", &m_CurStageIdx });
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "NextStage Idx", &m_NextStageIdx });
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "BGM Change 0:Off/1:On", &m_BGMTrigger });

}

CChangeStage::~CChangeStage()
{
}

void CChangeStage::SaveComponent(FILE* _File)
{
    fwrite(&m_CurStageIdx, sizeof(float), 1, _File);
    fwrite(&m_NextStageIdx, sizeof(float), 1, _File);
    fwrite(&m_BGMTrigger, sizeof(float), 1, _File);

}

void CChangeStage::LoadComponent(FILE* _File)
{
    fread(&m_CurStageIdx, sizeof(float), 1, _File);
    fread(&m_NextStageIdx, sizeof(float), 1, _File);
    fread(&m_BGMTrigger, sizeof(float), 1, _File);

}

void CChangeStage::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// Player¸¸ ÇØ´ç
	if (_OtherObject->GetLayerIdx() != 1)
		return;
	m_ColliderCheck = true;
	CRenderMgr::GetInst()->StartFadeInOut(1.f);
	CGameObject* pPlayer = CLevelMgr::GetInst()->FindObjectByName(L"Player");
	CPlayerScript* Script = (CPlayerScript*)pPlayer->GetScripts()[0];
	int HpID = Script->GetFontHp_Id();
	int MpID = Script->GetFontMp_Id();
	
	tPlayerInfo tData = Script->GetInfo();

	CDataMgr::GetInst()->SetPlayerData(tData.Cur_Hp, tData.Max_Hp, tData.Cur_Mp, tData.Max_Mp, tData.Atk);


	m_NextLevelPath = CPathMgr::GetInst()->GetContentPath() + L"Level\\" +CDataMgr::GetInst()->GetStageName(m_NextStageIdx);

	CFontMgr::GetInst()->StartFadeOut(HpID, 1.f);
	CFontMgr::GetInst()->StartFadeOut(MpID, 1.f);

	wstring CurBgm = CDataMgr::GetInst()->GetStageBGM(m_CurStageIdx);
	wstring NextBgm = CDataMgr::GetInst()->GetStageBGM(m_NextStageIdx);

	if (m_BGMTrigger != 0)
	{
		CSoundMgr::GetInst()->FadeOut(CurBgm, 0.5f);
		float volum = CSoundMgr::GetInst()->GetBGMVolume();
		CSoundMgr::GetInst()->FadeIn(NextBgm, 1.5f, volum);
	}

}

void CChangeStage::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CChangeStage::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CChangeStage::Begin()
{

}

void CChangeStage::Tick()
{
	if (!m_IsFadeOutCheck && CRenderMgr::GetInst()->IsFadeOutStart() && m_ColliderCheck)
	{
		m_IsFadeOutCheck = true;
		ChangeLevel(CLevelMgr::g_LevelLoad(m_NextLevelPath), LEVEL_STATE::PLAY);

	}
}