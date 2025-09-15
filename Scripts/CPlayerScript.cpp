#include "pch.h"
#include "CPlayerScript.h"

#include <Engine/CTaskMgr.h>
#include <Engine/CFontMgr.h>
#include <Engine/CSoundMgr.h>

#include <Engine/CDataMgr.h>

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_PrefObj(nullptr)
	, m_Create(false)
	, m_SlashTime(0.f)
	, m_Info{}
	, m_HpFontID(-1)
	, m_MpFontID(-1)
	, m_SlopeAngle(0.f)
{
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Player HP", &m_Info.Cur_Hp });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Player MP", &m_Info.Cur_Mp });
	//AddScriptParam(tScriptParam{ SCRIPT_PARAM::TEXTURE, "Test Texture", &m_TargetTex });

	AddScriptParam(tScriptParam{ SCRIPT_PARAM::PREFAB, "SlashParticle", &m_BustParticle });

	m_Info.Max_Hp = 300.f;
	m_Info.Max_Mp = 100.f;
	m_Info.IsBust = false;
	m_Info.IsRollnAttack = false;
	m_Info.PlayerSpeed = 200.f;
	m_Info.JumpCount = 0;
	
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Begin()
{
	m_BustParticle = CAssetMgr::GetInst()->Load<CPrefab>(L"Prefab\\Slash_Particle.pref", L"Prefab\\Slash_Particle.pref");
	m_Info.Cur_Hp = CDataMgr::GetInst()->GetPlayerData().Cur_Hp;
	m_Info.Cur_Mp = CDataMgr::GetInst()->GetPlayerData().Cur_Mp;

	wstring hpText = to_wstring(m_Info.Cur_Hp) + L"/" + to_wstring(m_Info.Max_Hp);
	wstring mpText = to_wstring(m_Info.Cur_Mp) + L"/" + to_wstring(m_Info.Max_Mp);

	m_HpFontID = CFontMgr::GetInst()->RegisterSimultaneousFadeText(hpText, 124.f, 44.f, 20.f, FONT_RGBA(255, 255, 255, 255),1.f);
	m_MpFontID = CFontMgr::GetInst()->RegisterSimultaneousFadeText(mpText, 124.f, 84.f, 20.f, FONT_RGBA(255, 255, 255, 255),1.f);

}

void CPlayerScript::Tick()
{

	if (KEY_TAP(KEY::LEFT)&&!m_Info.IsRollnAttack)
	{
		Transform()->SetRelativeScale(-230.f, 230.f, 1.f);
	}
	else if (KEY_TAP(KEY::RIGHT) && !m_Info.IsRollnAttack)
	{
		Transform()->SetRelativeScale(230.f, 230.f, 1.f);
	}


	// Tap 할때 Animation 적용
	if (!(KEY_PRESSED(KEY::RIGHT)) && !m_Info.IsRollnAttack)
	{
		if (KEY_TAP(KEY::LEFT))
		{
			RigidBody2D()->SetVelocity(Vec3(-50.f, 0.f, 0.f));
			if (RigidBody2D()->IsGround())
				StateMachine()->ChangeState(L"CPlayerIdletoRun");

		}
	}
	if (!(KEY_PRESSED(KEY::RIGHT)) && !m_Info.IsRollnAttack)
	{
		if (KEY_TAP(KEY::RIGHT))
		{
			RigidBody2D()->SetVelocity(Vec3(50.f, 0.f, 0.f));
			if (RigidBody2D()->IsGround())
				StateMachine()->ChangeState(L"CPlayerIdletoRun");

		}
	}

	// Release 할때 Animation 적용
	if (KEY_RELEASED(KEY::LEFT) && !m_Info.IsRollnAttack)
	{	
		// 공중에서는 바뀌면 안됨
		if (RigidBody2D()->IsGround() && RigidBody2D()->IsMove())
		{
			if(StateMachine()->GetCurStateName() == L"CPlayerIdletoRun")
				StateMachine()->ChangeState(L"CPlayerIdle");
			else
				StateMachine()->ChangeState(L"CPlayerBrake");
		}




	}
	if (KEY_RELEASED(KEY::RIGHT) && !m_Info.IsRollnAttack)
	{
		// 공중에서는 바뀌면 안됨
		if (RigidBody2D()->IsGround() && RigidBody2D()->IsMove())
		{
			if (StateMachine()->GetCurStateName() == L"CPlayerIdletoRun")
				StateMachine()->ChangeState(L"CPlayerIdle");
			else
				StateMachine()->ChangeState(L"CPlayerBrake");
		}
	}


	// Press 할때 이동적용
	if (KEY_PRESSED(KEY::LEFT) && !m_Info.IsRollnAttack)
	{
		//RigidBody2D()->AddForce(Vec3(-600.f, 0.f, 0.f));
		// 공중에선 적용 x
		if (RigidBody2D()->IsGround() && KEY_TAP(KEY::RIGHT))
		{
			StateMachine()->ChangeState(L"CPlayerBrake");
		}
		if (StateMachine()->GetCurStateName() == L"CPlayerIdle")
		{
			if (!(KEY_PRESSED(KEY::RIGHT)))
			{
				StateMachine()->ChangeState(L"CPlayerIdletoRun");
			}
		}

	}
	if (KEY_PRESSED(KEY::RIGHT) && !m_Info.IsRollnAttack)
	{
		//RigidBody2D()->AddForce(Vec3(600.f, 0.f, 0.f));
		if (RigidBody2D()->IsGround() && KEY_TAP(KEY::LEFT))
		{
			StateMachine()->ChangeState(L"CPlayerBrake");
		}
		if (StateMachine()->GetCurStateName() == L"CPlayerIdle")
		{
			if (!(KEY_PRESSED(KEY::LEFT)))
			{
				StateMachine()->ChangeState(L"CPlayerIdletoRun");
			}
		}
	}
	// 점프
	if (KEY_TAP(KEY::SPACE)  && !m_Info.IsRollnAttack && !(KEY_PRESSED(KEY::DOWN)))
	{
		if (m_Info.JumpCount == 0)
		{
			RigidBody2D()->Jump();
			StateMachine()->ChangeState(L"CPlayerJumpUp");
			m_Info.JumpCount = 1;
		}
	}

	// 공격
	if (KEY_TAP(KEY::A) && !m_Info.IsRollnAttack)
	{
		if (KEY_PRESSED(KEY::LEFT))
			Transform()->SetRelativeScale(-230.f, 230.f, 1.f);
		else if(KEY_PRESSED(KEY::RIGHT))
			Transform()->SetRelativeScale(230.f, 230.f, 1.f);

		StateMachine()->ChangeState(L"CPlayerAttack1");
	}

	// Test
	if (KEY_TAP(KEY::NUM_5))
	{

		//CDataMgr::GetInst()->SetPlayerData(m_Info.Cur_Hp, m_Info.Max_Hp, m_Info.Cur_Mp, m_Info.Max_Mp, 20, 0);
		//int a = CDataMgr::GetInst()->SavePlayerData(CPathMgr::GetInst()->GetContentPath() + L"Save\\PlayerData.data");
		CDataMgr::GetInst()->SetStageData();
		CDataMgr::GetInst()->SaveStageData(CPathMgr::GetInst()->GetContentPath() + L"Save\\StageData.data");
	}


	// Bust 시 Particle 생성
	if (StateMachine()->GetCurStateName() == L"CPlayerAttack3" && m_Info.IsBust && !m_Create)
	{
		m_PrefObj = Instantiate_Edit(m_BustParticle, Vec3(Transform()->GetRelativePos().x+300.f, Transform()->GetRelativePos().y + 400.f, 0.f), 2);
		m_Create = true;
	}
	if (m_Create)
		m_SlashTime += DT;
	if (m_SlashTime > 0.8f)
	{
		DestroyObject(m_PrefObj);
		m_PrefObj = nullptr;
		m_Create = false;
		m_SlashTime = 0.f;
	}

	if (RigidBody2D()->IsGround() && m_Info.JumpCount >0)
	{
		m_Info.JumpCount = 0;
	}

}

void CPlayerScript::CalcHp(int _Hp)
{
	m_Info.Cur_Hp += _Hp;
	if (m_Info.Cur_Hp >= m_Info.Max_Hp)
		m_Info.Cur_Hp = m_Info.Max_Hp;
	else if (m_Info.Cur_Hp <= 0)
		m_Info.Cur_Hp = 0;

	if (m_Info.Cur_Hp == 40)
		CSoundMgr::GetInst()->PlayFX(L"HPLow", 0.5f);

	// HP 변경시 텍스트 업데이트
	wstring hpText = to_wstring(m_Info.Cur_Hp) + L"/" + to_wstring(m_Info.Max_Hp);

	float xPos = 124.f;
	if (m_Info.Cur_Hp < 100) xPos += 10.f;      // 2자리수
	if (m_Info.Cur_Hp < 10) xPos += 10.f;       // 1자리수

	CFontMgr::GetInst()->UpdatePosition(m_HpFontID, xPos, 44.f);
	CFontMgr::GetInst()->UpdateText(m_HpFontID, hpText);

}

void CPlayerScript::CalcMp(int _Mp)
{
	m_Info.Cur_Mp += _Mp;
	if (m_Info.Cur_Mp >= 100)
		m_Info.Cur_Mp = 100;
	else if (m_Info.Cur_Mp <= 0)
		m_Info.Cur_Mp = 0;


	// MP 변경시 텍스트 업데이트
	wstring mpText = to_wstring(m_Info.Cur_Mp) + L"/100";

	// 숫자의 자릿수에 따라 x좌표 조정 (기준점 124.f)
	float xPos = 124.f;
	if (m_Info.Cur_Mp < 100) xPos += 10.f;      // 2자리수
	if (m_Info.Cur_Mp < 10) xPos += 10.f;       // 1자리수

	CFontMgr::GetInst()->UpdatePosition(m_MpFontID, xPos, 84.f);
	CFontMgr::GetInst()->UpdateText(m_MpFontID, mpText);
}


void CPlayerScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{


	
	
}

void CPlayerScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

	
}

void CPlayerScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

}


void CPlayerScript::SaveComponent(FILE* _File)
{

}

void CPlayerScript::LoadComponent(FILE* _File)
{

}