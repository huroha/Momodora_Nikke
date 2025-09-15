#include "pch.h"
#include "CBGBellRing.h"

#include <Engine/CSoundMgr.h>
#include <Engine/CLevelMgr.h>
#include "CHpRestore.h"

#include "CHpControll.h"
#include "CMpControll.h"
#include "CPlayerScript.h"


#define POSTOBJ m_vecObj[0]
#define BELLOBJ m_vecObj[1]
#define RESTOREOBJ m_vecObj[2]
#define HPOBJ m_vecObj[3]
#define MPOBJ m_vecObj[4]
#define HPUIBAR m_vecObj[5]


CBGBellRing::CBGBellRing()
	: CScript((UINT)SCRIPT_TYPE::BGBELLRING)
	, m_IsCollision(false)
	, m_Time(0.f)
{
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::PREFAB, "PostProcess", &m_Prefab });
}

CBGBellRing::~CBGBellRing()
{
	m_vecObj.clear();
}


void CBGBellRing::SaveComponent(FILE* _File)
{
}

void CBGBellRing::LoadComponent(FILE* _File)
{
}

void CBGBellRing::Begin()
{
	m_Prefab = CAssetMgr::GetInst()->Load<CPrefab>(L"Prefab\\Wave_PostProcess.pref", L"Prefab\\Wave_PostProcess.pref");
	
	// 0. PostObj 1. BellObj 2. RestoreTex 3. HpTex 4. MpTex
	CGameObject* pObj;
	pObj = nullptr;
	m_vecObj.push_back(pObj);
	pObj = CLevelMgr::GetInst()->FindObjectByName(L"BG_Bell_Tree");
	m_vecObj.push_back(pObj);
	pObj = CLevelMgr::GetInst()->FindObjectByName(L"MpRestore");
	m_vecObj.push_back(pObj);
	pObj = CLevelMgr::GetInst()->FindObjectByName(L"HP_Remain");
	m_vecObj.push_back(pObj);
	pObj = CLevelMgr::GetInst()->FindObjectByName(L"MP_Remain");
	m_vecObj.push_back(pObj);
	pObj = CLevelMgr::GetInst()->FindObjectByName(L"HP_Bar");
	m_vecObj.push_back(pObj);


	
}

void CBGBellRing::Tick()
{

	if (m_IsCollision)
	{
		m_Time += DT;

		if (m_Time > 2.3f)
		{
			DestroyObject(POSTOBJ);
			POSTOBJ = nullptr;
			m_IsCollision = false;
			m_Time = 0;
			HPUIBAR->FlipbookPlayer()->Play(0, 1, false);
		}
	}

}

void CBGBellRing::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	
	m_IsCollision = true;
	if (POSTOBJ == nullptr)
	{
		FlipbookPlayer()->Play(0, 12, false);
		Vec3 Position = BELLOBJ->Transform()->GetRelativePos();
		POSTOBJ = Instantiate_Edit(m_Prefab, Vec3(Position.x, Position.y +30,0.f), 2);
		POSTOBJ->Transform()->SetSpawnTime(g_Data.Time);

		_OtherObject->GetParent()->FlipbookPlayer()->StartHitEffect(1.2f, Vec3(0.f, 0.4f, 0.7f));

		((CHpRestore*)RESTOREOBJ->GetScripts()[0])->SetEffect(true);

		// 실제 플레이어의 HP/MP 업데이트
		int Max_Hp = ((CPlayerScript*)_OtherObject->GetParent()->GetScripts()[0])->GetInfo().Max_Hp;

		((CPlayerScript*)_OtherObject->GetParent()->GetScripts()[0])->CalcHp(Max_Hp);
		((CPlayerScript*)_OtherObject->GetParent()->GetScripts()[0])->CalcMp(100);


		// UI 텍스쳐도 100%로 설정 (양수 값을 전달하여 scale을 최대로)
		((CHpControll*)HPOBJ->GetScripts()[0])->SettingHp(Max_Hp);
		((CMpControll*)MPOBJ->GetScripts()[0])->SettingMp(100);

		HPUIBAR->FlipbookPlayer()->Play(1, 1, false);

		CSoundMgr::GetInst()->PlayFX(L"Bell", 0.5f);
	}



}

void CBGBellRing::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{


}

void CBGBellRing::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}
