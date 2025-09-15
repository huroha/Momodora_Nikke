#include "pch.h"
#include "CStateMgr.h"

#include "CBombBoom.h"
#include "CBombIdle.h"
#include "CBoomMove.h"
#include "CBoss1Attack.h"
#include "CBoss1Attack2.h"
#include "CBoss1Attack3.h"
#include "CBoss1Dead.h"
#include "CBoss1Idle.h"
#include "CBoss1Idle2.h"
#include "CBoss1None.h"
#include "CMonster1Dead.h"
#include "CMonster2Attack.h"
#include "CMonster2Dead.h"
#include "CMonster2Hit.h"
#include "CMonster2Idle.h"
#include "CMonster3Attack.h"
#include "CMonster3Dead.h"
#include "CMonster3Hit.h"
#include "CMonster3Idle.h"
#include "CMonsterAttack.h"
#include "CMonsterHit.h"
#include "CMonsterIdle.h"
#include "CMonsterRun.h"
#include "CNpcEnter_d1.h"
#include "CNpcEnter_d2.h"
#include "CNpcEnter_d3.h"
#include "CNpcTemD1.h"
#include "CNpcTemD2.h"
#include "CNpcTemD3.h"
#include "CNpcTown1_d1.h"
#include "CNpcTown1_d2.h"
#include "CNpcTown1_d3.h"
#include "CPauseMain.h"
#include "CPauseMemo.h"
#include "CPauseOption.h"
#include "CPlayerAttack1.h"
#include "CPlayerAttack2.h"
#include "CPlayerAttack3.h"
#include "CPlayerBrake.h"
#include "CPlayerHitState.h"
#include "CPlayerIdle.h"
#include "CPlayerIdletoRun.h"
#include "CPlayerJumpDown.h"
#include "CPlayerJumpUp.h"
#include "CPlayerLand.h"
#include "CPlayerRoll.h"
#include "CPlayerRun.h"
#include "CQuakeEffect.h"
#include "CQuakeEffectNone.h"

void CStateMgr::GetStateInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBombBoom");
	_vec.push_back(L"CBombIdle");
	_vec.push_back(L"CBoomMove");
	_vec.push_back(L"CBoss1Attack");
	_vec.push_back(L"CBoss1Attack2");
	_vec.push_back(L"CBoss1Attack3");
	_vec.push_back(L"CBoss1Dead");
	_vec.push_back(L"CBoss1Idle");
	_vec.push_back(L"CBoss1Idle2");
	_vec.push_back(L"CBoss1None");
	_vec.push_back(L"CMonster1Dead");
	_vec.push_back(L"CMonster2Attack");
	_vec.push_back(L"CMonster2Dead");
	_vec.push_back(L"CMonster2Hit");
	_vec.push_back(L"CMonster2Idle");
	_vec.push_back(L"CMonster3Attack");
	_vec.push_back(L"CMonster3Dead");
	_vec.push_back(L"CMonster3Hit");
	_vec.push_back(L"CMonster3Idle");
	_vec.push_back(L"CMonsterAttack");
	_vec.push_back(L"CMonsterHit");
	_vec.push_back(L"CMonsterIdle");
	_vec.push_back(L"CMonsterRun");
	_vec.push_back(L"CNpcEnter_d1");
	_vec.push_back(L"CNpcEnter_d2");
	_vec.push_back(L"CNpcEnter_d3");
	_vec.push_back(L"CNpcTemD1");
	_vec.push_back(L"CNpcTemD2");
	_vec.push_back(L"CNpcTemD3");
	_vec.push_back(L"CNpcTown1_d1");
	_vec.push_back(L"CNpcTown1_d2");
	_vec.push_back(L"CNpcTown1_d3");
	_vec.push_back(L"CPauseMain");
	_vec.push_back(L"CPauseMemo");
	_vec.push_back(L"CPauseOption");
	_vec.push_back(L"CPlayerAttack1");
	_vec.push_back(L"CPlayerAttack2");
	_vec.push_back(L"CPlayerAttack3");
	_vec.push_back(L"CPlayerBrake");
	_vec.push_back(L"CPlayerHitState");
	_vec.push_back(L"CPlayerIdle");
	_vec.push_back(L"CPlayerIdletoRun");
	_vec.push_back(L"CPlayerJumpDown");
	_vec.push_back(L"CPlayerJumpUp");
	_vec.push_back(L"CPlayerLand");
	_vec.push_back(L"CPlayerRoll");
	_vec.push_back(L"CPlayerRun");
	_vec.push_back(L"CQuakeEffect");
	_vec.push_back(L"CQuakeEffectNone");
}

CState * CStateMgr::GetState(const wstring& _strStateName)
{
	if (L"CBombBoom" == _strStateName)
		return new CBombBoom;
	if (L"CBombIdle" == _strStateName)
		return new CBombIdle;
	if (L"CBoomMove" == _strStateName)
		return new CBoomMove;
	if (L"CBoss1Attack" == _strStateName)
		return new CBoss1Attack;
	if (L"CBoss1Attack2" == _strStateName)
		return new CBoss1Attack2;
	if (L"CBoss1Attack3" == _strStateName)
		return new CBoss1Attack3;
	if (L"CBoss1Dead" == _strStateName)
		return new CBoss1Dead;
	if (L"CBoss1Idle" == _strStateName)
		return new CBoss1Idle;
	if (L"CBoss1Idle2" == _strStateName)
		return new CBoss1Idle2;
	if (L"CBoss1None" == _strStateName)
		return new CBoss1None;
	if (L"CMonster1Dead" == _strStateName)
		return new CMonster1Dead;
	if (L"CMonster2Attack" == _strStateName)
		return new CMonster2Attack;
	if (L"CMonster2Dead" == _strStateName)
		return new CMonster2Dead;
	if (L"CMonster2Hit" == _strStateName)
		return new CMonster2Hit;
	if (L"CMonster2Idle" == _strStateName)
		return new CMonster2Idle;
	if (L"CMonster3Attack" == _strStateName)
		return new CMonster3Attack;
	if (L"CMonster3Dead" == _strStateName)
		return new CMonster3Dead;
	if (L"CMonster3Hit" == _strStateName)
		return new CMonster3Hit;
	if (L"CMonster3Idle" == _strStateName)
		return new CMonster3Idle;
	if (L"CMonsterAttack" == _strStateName)
		return new CMonsterAttack;
	if (L"CMonsterHit" == _strStateName)
		return new CMonsterHit;
	if (L"CMonsterIdle" == _strStateName)
		return new CMonsterIdle;
	if (L"CMonsterRun" == _strStateName)
		return new CMonsterRun;
	if (L"CNpcEnter_d1" == _strStateName)
		return new CNpcEnter_d1;
	if (L"CNpcEnter_d2" == _strStateName)
		return new CNpcEnter_d2;
	if (L"CNpcEnter_d3" == _strStateName)
		return new CNpcEnter_d3;
	if (L"CNpcTemD1" == _strStateName)
		return new CNpcTemD1;
	if (L"CNpcTemD2" == _strStateName)
		return new CNpcTemD2;
	if (L"CNpcTemD3" == _strStateName)
		return new CNpcTemD3;
	if (L"CNpcTown1_d1" == _strStateName)
		return new CNpcTown1_d1;
	if (L"CNpcTown1_d2" == _strStateName)
		return new CNpcTown1_d2;
	if (L"CNpcTown1_d3" == _strStateName)
		return new CNpcTown1_d3;
	if (L"CPauseMain" == _strStateName)
		return new CPauseMain;
	if (L"CPauseMemo" == _strStateName)
		return new CPauseMemo;
	if (L"CPauseOption" == _strStateName)
		return new CPauseOption;
	if (L"CPlayerAttack1" == _strStateName)
		return new CPlayerAttack1;
	if (L"CPlayerAttack2" == _strStateName)
		return new CPlayerAttack2;
	if (L"CPlayerAttack3" == _strStateName)
		return new CPlayerAttack3;
	if (L"CPlayerBrake" == _strStateName)
		return new CPlayerBrake;
	if (L"CPlayerHitState" == _strStateName)
		return new CPlayerHitState;
	if (L"CPlayerIdle" == _strStateName)
		return new CPlayerIdle;
	if (L"CPlayerIdletoRun" == _strStateName)
		return new CPlayerIdletoRun;
	if (L"CPlayerJumpDown" == _strStateName)
		return new CPlayerJumpDown;
	if (L"CPlayerJumpUp" == _strStateName)
		return new CPlayerJumpUp;
	if (L"CPlayerLand" == _strStateName)
		return new CPlayerLand;
	if (L"CPlayerRoll" == _strStateName)
		return new CPlayerRoll;
	if (L"CPlayerRun" == _strStateName)
		return new CPlayerRun;
	if (L"CQuakeEffect" == _strStateName)
		return new CQuakeEffect;
	if (L"CQuakeEffectNone" == _strStateName)
		return new CQuakeEffectNone;
	return nullptr;
}

CState * CStateMgr::GetState(UINT _iStateType)
{
	switch (_iStateType)
	{
	case (UINT)STATE_TYPE::BOMBBOOM:
		return new CBombBoom;
		break;
	case (UINT)STATE_TYPE::BOMBIDLE:
		return new CBombIdle;
		break;
	case (UINT)STATE_TYPE::BOOMMOVE:
		return new CBoomMove;
		break;
	case (UINT)STATE_TYPE::BOSS1ATTACK:
		return new CBoss1Attack;
		break;
	case (UINT)STATE_TYPE::BOSS1ATTACK2:
		return new CBoss1Attack2;
		break;
	case (UINT)STATE_TYPE::BOSS1ATTACK3:
		return new CBoss1Attack3;
		break;
	case (UINT)STATE_TYPE::BOSS1DEAD:
		return new CBoss1Dead;
		break;
	case (UINT)STATE_TYPE::BOSS1IDLE:
		return new CBoss1Idle;
		break;
	case (UINT)STATE_TYPE::BOSS1IDLE2:
		return new CBoss1Idle2;
		break;
	case (UINT)STATE_TYPE::BOSS1NONE:
		return new CBoss1None;
		break;
	case (UINT)STATE_TYPE::MONSTER1DEAD:
		return new CMonster1Dead;
		break;
	case (UINT)STATE_TYPE::MONSTER2ATTACK:
		return new CMonster2Attack;
		break;
	case (UINT)STATE_TYPE::MONSTER2DEAD:
		return new CMonster2Dead;
		break;
	case (UINT)STATE_TYPE::MONSTER2HIT:
		return new CMonster2Hit;
		break;
	case (UINT)STATE_TYPE::MONSTER2IDLE:
		return new CMonster2Idle;
		break;
	case (UINT)STATE_TYPE::MONSTER3ATTACK:
		return new CMonster3Attack;
		break;
	case (UINT)STATE_TYPE::MONSTER3DEAD:
		return new CMonster3Dead;
		break;
	case (UINT)STATE_TYPE::MONSTER3HIT:
		return new CMonster3Hit;
		break;
	case (UINT)STATE_TYPE::MONSTER3IDLE:
		return new CMonster3Idle;
		break;
	case (UINT)STATE_TYPE::MONSTERATTACK:
		return new CMonsterAttack;
		break;
	case (UINT)STATE_TYPE::MONSTERHIT:
		return new CMonsterHit;
		break;
	case (UINT)STATE_TYPE::MONSTERIDLE:
		return new CMonsterIdle;
		break;
	case (UINT)STATE_TYPE::MONSTERRUN:
		return new CMonsterRun;
		break;
	case (UINT)STATE_TYPE::NPCENTER_D1:
		return new CNpcEnter_d1;
		break;
	case (UINT)STATE_TYPE::NPCENTER_D2:
		return new CNpcEnter_d2;
		break;
	case (UINT)STATE_TYPE::NPCENTER_D3:
		return new CNpcEnter_d3;
		break;
	case (UINT)STATE_TYPE::NPCTEMD1:
		return new CNpcTemD1;
		break;
	case (UINT)STATE_TYPE::NPCTEMD2:
		return new CNpcTemD2;
		break;
	case (UINT)STATE_TYPE::NPCTEMD3:
		return new CNpcTemD3;
		break;
	case (UINT)STATE_TYPE::NPCTOWN1_D1:
		return new CNpcTown1_d1;
		break;
	case (UINT)STATE_TYPE::NPCTOWN1_D2:
		return new CNpcTown1_d2;
		break;
	case (UINT)STATE_TYPE::NPCTOWN1_D3:
		return new CNpcTown1_d3;
		break;
	case (UINT)STATE_TYPE::PAUSEMAIN:
		return new CPauseMain;
		break;
	case (UINT)STATE_TYPE::PAUSEMEMO:
		return new CPauseMemo;
		break;
	case (UINT)STATE_TYPE::PAUSEOPTION:
		return new CPauseOption;
		break;
	case (UINT)STATE_TYPE::PLAYERATTACK1:
		return new CPlayerAttack1;
		break;
	case (UINT)STATE_TYPE::PLAYERATTACK2:
		return new CPlayerAttack2;
		break;
	case (UINT)STATE_TYPE::PLAYERATTACK3:
		return new CPlayerAttack3;
		break;
	case (UINT)STATE_TYPE::PLAYERBRAKE:
		return new CPlayerBrake;
		break;
	case (UINT)STATE_TYPE::PLAYERHITSTATE:
		return new CPlayerHitState;
		break;
	case (UINT)STATE_TYPE::PLAYERIDLE:
		return new CPlayerIdle;
		break;
	case (UINT)STATE_TYPE::PLAYERIDLETORUN:
		return new CPlayerIdletoRun;
		break;
	case (UINT)STATE_TYPE::PLAYERJUMPDOWN:
		return new CPlayerJumpDown;
		break;
	case (UINT)STATE_TYPE::PLAYERJUMPUP:
		return new CPlayerJumpUp;
		break;
	case (UINT)STATE_TYPE::PLAYERLAND:
		return new CPlayerLand;
		break;
	case (UINT)STATE_TYPE::PLAYERROLL:
		return new CPlayerRoll;
		break;
	case (UINT)STATE_TYPE::PLAYERRUN:
		return new CPlayerRun;
		break;
	case (UINT)STATE_TYPE::QUAKEEFFECT:
		return new CQuakeEffect;
		break;
	case (UINT)STATE_TYPE::QUAKEEFFECTNONE:
		return new CQuakeEffectNone;
		break;
	}
	return nullptr;
}

const wchar_t * CStateMgr::GetStateName(CState * _pState)
{
	switch ((STATE_TYPE)_pState->GetStateType())
	{
	case STATE_TYPE::BOMBBOOM:
		return L"CBombBoom";
		break;

	case STATE_TYPE::BOMBIDLE:
		return L"CBombIdle";
		break;

	case STATE_TYPE::BOOMMOVE:
		return L"CBoomMove";
		break;

	case STATE_TYPE::BOSS1ATTACK:
		return L"CBoss1Attack";
		break;

	case STATE_TYPE::BOSS1ATTACK2:
		return L"CBoss1Attack2";
		break;

	case STATE_TYPE::BOSS1ATTACK3:
		return L"CBoss1Attack3";
		break;

	case STATE_TYPE::BOSS1DEAD:
		return L"CBoss1Dead";
		break;

	case STATE_TYPE::BOSS1IDLE:
		return L"CBoss1Idle";
		break;

	case STATE_TYPE::BOSS1IDLE2:
		return L"CBoss1Idle2";
		break;

	case STATE_TYPE::BOSS1NONE:
		return L"CBoss1None";
		break;

	case STATE_TYPE::MONSTER1DEAD:
		return L"CMonster1Dead";
		break;

	case STATE_TYPE::MONSTER2ATTACK:
		return L"CMonster2Attack";
		break;

	case STATE_TYPE::MONSTER2DEAD:
		return L"CMonster2Dead";
		break;

	case STATE_TYPE::MONSTER2HIT:
		return L"CMonster2Hit";
		break;

	case STATE_TYPE::MONSTER2IDLE:
		return L"CMonster2Idle";
		break;

	case STATE_TYPE::MONSTER3ATTACK:
		return L"CMonster3Attack";
		break;

	case STATE_TYPE::MONSTER3DEAD:
		return L"CMonster3Dead";
		break;

	case STATE_TYPE::MONSTER3HIT:
		return L"CMonster3Hit";
		break;

	case STATE_TYPE::MONSTER3IDLE:
		return L"CMonster3Idle";
		break;

	case STATE_TYPE::MONSTERATTACK:
		return L"CMonsterAttack";
		break;

	case STATE_TYPE::MONSTERHIT:
		return L"CMonsterHit";
		break;

	case STATE_TYPE::MONSTERIDLE:
		return L"CMonsterIdle";
		break;

	case STATE_TYPE::MONSTERRUN:
		return L"CMonsterRun";
		break;

	case STATE_TYPE::NPCENTER_D1:
		return L"CNpcEnter_d1";
		break;

	case STATE_TYPE::NPCENTER_D2:
		return L"CNpcEnter_d2";
		break;

	case STATE_TYPE::NPCENTER_D3:
		return L"CNpcEnter_d3";
		break;

	case STATE_TYPE::NPCTEMD1:
		return L"CNpcTemD1";
		break;

	case STATE_TYPE::NPCTEMD2:
		return L"CNpcTemD2";
		break;

	case STATE_TYPE::NPCTEMD3:
		return L"CNpcTemD3";
		break;

	case STATE_TYPE::NPCTOWN1_D1:
		return L"CNpcTown1_d1";
		break;

	case STATE_TYPE::NPCTOWN1_D2:
		return L"CNpcTown1_d2";
		break;

	case STATE_TYPE::NPCTOWN1_D3:
		return L"CNpcTown1_d3";
		break;

	case STATE_TYPE::PAUSEMAIN:
		return L"CPauseMain";
		break;

	case STATE_TYPE::PAUSEMEMO:
		return L"CPauseMemo";
		break;

	case STATE_TYPE::PAUSEOPTION:
		return L"CPauseOption";
		break;

	case STATE_TYPE::PLAYERATTACK1:
		return L"CPlayerAttack1";
		break;

	case STATE_TYPE::PLAYERATTACK2:
		return L"CPlayerAttack2";
		break;

	case STATE_TYPE::PLAYERATTACK3:
		return L"CPlayerAttack3";
		break;

	case STATE_TYPE::PLAYERBRAKE:
		return L"CPlayerBrake";
		break;

	case STATE_TYPE::PLAYERHITSTATE:
		return L"CPlayerHitState";
		break;

	case STATE_TYPE::PLAYERIDLE:
		return L"CPlayerIdle";
		break;

	case STATE_TYPE::PLAYERIDLETORUN:
		return L"CPlayerIdletoRun";
		break;

	case STATE_TYPE::PLAYERJUMPDOWN:
		return L"CPlayerJumpDown";
		break;

	case STATE_TYPE::PLAYERJUMPUP:
		return L"CPlayerJumpUp";
		break;

	case STATE_TYPE::PLAYERLAND:
		return L"CPlayerLand";
		break;

	case STATE_TYPE::PLAYERROLL:
		return L"CPlayerRoll";
		break;

	case STATE_TYPE::PLAYERRUN:
		return L"CPlayerRun";
		break;

	case STATE_TYPE::QUAKEEFFECT:
		return L"CQuakeEffect";
		break;

	case STATE_TYPE::QUAKEEFFECTNONE:
		return L"CQuakeEffectNone";
		break;

	}
	return nullptr;
}