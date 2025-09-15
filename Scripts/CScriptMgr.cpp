#include "pch.h"
#include "CScriptMgr.h"

#include "CBGBellRing.h"
#include "CBGMove.h"
#include "CBoss1Hit.h"
#include "CBoss1HpControll.h"
#include "CBoss1HpEffect.h"
#include "CBoss1Script.h"
#include "CBoss1SetTarget.h"
#include "CBossStageScript.h"
#include "CCameraScript.h"
#include "CCeilingCheck.h"
#include "CChangeStage.h"
#include "CCopyRight.h"
#include "CDoroMove.h"
#include "CFontPos.h"
#include "CGroundBoard.h"
#include "CGroundFix.h"
#include "CGroundScript.h"
#include "CGroundSlope.h"
#include "CGroundWall.h"
#include "CHpControll.h"
#include "CHpReduceFx.h"
#include "CHpRestore.h"
#include "CJellyJump.h"
#include "CMissileScript.h"
#include "CMonster1Hit.h"
#include "CMonster1Script.h"
#include "CMonster2HitScript.h"
#include "CMonster2Script.h"
#include "CMonster2Target.h"
#include "CMonster3HitScript.h"
#include "CMonster3Script.h"
#include "CMonster3SetTarget.h"
#include "CMonsterSetTarget.h"
#include "CMpControll.h"
#include "CNpcEnterDialog.h"
#include "CNpcTemple.h"
#include "CNpcTempleDialog.h"
#include "CNpcTownDialog.h"
#include "CParticleLeaf.h"
#include "CPauseFont.h"
#include "CPlayButtonScript.h"
#include "CPlayerAttackScript.h"
#include "CPlayerHit.h"
#include "CPlayerScript.h"
#include "CWaterSplash.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBGBellRing");
	_vec.push_back(L"CBGMove");
	_vec.push_back(L"CBoss1Hit");
	_vec.push_back(L"CBoss1HpControll");
	_vec.push_back(L"CBoss1HpEffect");
	_vec.push_back(L"CBoss1Script");
	_vec.push_back(L"CBoss1SetTarget");
	_vec.push_back(L"CBossStageScript");
	_vec.push_back(L"CCameraScript");
	_vec.push_back(L"CCeilingCheck");
	_vec.push_back(L"CChangeStage");
	_vec.push_back(L"CCopyRight");
	_vec.push_back(L"CDoroMove");
	_vec.push_back(L"CFontPos");
	_vec.push_back(L"CGroundBoard");
	_vec.push_back(L"CGroundFix");
	_vec.push_back(L"CGroundScript");
	_vec.push_back(L"CGroundSlope");
	_vec.push_back(L"CGroundWall");
	_vec.push_back(L"CHpControll");
	_vec.push_back(L"CHpReduceFx");
	_vec.push_back(L"CHpRestore");
	_vec.push_back(L"CJellyJump");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonster1Hit");
	_vec.push_back(L"CMonster1Script");
	_vec.push_back(L"CMonster2HitScript");
	_vec.push_back(L"CMonster2Script");
	_vec.push_back(L"CMonster2Target");
	_vec.push_back(L"CMonster3HitScript");
	_vec.push_back(L"CMonster3Script");
	_vec.push_back(L"CMonster3SetTarget");
	_vec.push_back(L"CMonsterSetTarget");
	_vec.push_back(L"CMpControll");
	_vec.push_back(L"CNpcEnterDialog");
	_vec.push_back(L"CNpcTemple");
	_vec.push_back(L"CNpcTempleDialog");
	_vec.push_back(L"CNpcTownDialog");
	_vec.push_back(L"CParticleLeaf");
	_vec.push_back(L"CPauseFont");
	_vec.push_back(L"CPlayButtonScript");
	_vec.push_back(L"CPlayerAttackScript");
	_vec.push_back(L"CPlayerHit");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CWaterSplash");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBGBellRing" == _strScriptName)
		return new CBGBellRing;
	if (L"CBGMove" == _strScriptName)
		return new CBGMove;
	if (L"CBoss1Hit" == _strScriptName)
		return new CBoss1Hit;
	if (L"CBoss1HpControll" == _strScriptName)
		return new CBoss1HpControll;
	if (L"CBoss1HpEffect" == _strScriptName)
		return new CBoss1HpEffect;
	if (L"CBoss1Script" == _strScriptName)
		return new CBoss1Script;
	if (L"CBoss1SetTarget" == _strScriptName)
		return new CBoss1SetTarget;
	if (L"CBossStageScript" == _strScriptName)
		return new CBossStageScript;
	if (L"CCameraScript" == _strScriptName)
		return new CCameraScript;
	if (L"CCeilingCheck" == _strScriptName)
		return new CCeilingCheck;
	if (L"CChangeStage" == _strScriptName)
		return new CChangeStage;
	if (L"CCopyRight" == _strScriptName)
		return new CCopyRight;
	if (L"CDoroMove" == _strScriptName)
		return new CDoroMove;
	if (L"CFontPos" == _strScriptName)
		return new CFontPos;
	if (L"CGroundBoard" == _strScriptName)
		return new CGroundBoard;
	if (L"CGroundFix" == _strScriptName)
		return new CGroundFix;
	if (L"CGroundScript" == _strScriptName)
		return new CGroundScript;
	if (L"CGroundSlope" == _strScriptName)
		return new CGroundSlope;
	if (L"CGroundWall" == _strScriptName)
		return new CGroundWall;
	if (L"CHpControll" == _strScriptName)
		return new CHpControll;
	if (L"CHpReduceFx" == _strScriptName)
		return new CHpReduceFx;
	if (L"CHpRestore" == _strScriptName)
		return new CHpRestore;
	if (L"CJellyJump" == _strScriptName)
		return new CJellyJump;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonster1Hit" == _strScriptName)
		return new CMonster1Hit;
	if (L"CMonster1Script" == _strScriptName)
		return new CMonster1Script;
	if (L"CMonster2HitScript" == _strScriptName)
		return new CMonster2HitScript;
	if (L"CMonster2Script" == _strScriptName)
		return new CMonster2Script;
	if (L"CMonster2Target" == _strScriptName)
		return new CMonster2Target;
	if (L"CMonster3HitScript" == _strScriptName)
		return new CMonster3HitScript;
	if (L"CMonster3Script" == _strScriptName)
		return new CMonster3Script;
	if (L"CMonster3SetTarget" == _strScriptName)
		return new CMonster3SetTarget;
	if (L"CMonsterSetTarget" == _strScriptName)
		return new CMonsterSetTarget;
	if (L"CMpControll" == _strScriptName)
		return new CMpControll;
	if (L"CNpcEnterDialog" == _strScriptName)
		return new CNpcEnterDialog;
	if (L"CNpcTemple" == _strScriptName)
		return new CNpcTemple;
	if (L"CNpcTempleDialog" == _strScriptName)
		return new CNpcTempleDialog;
	if (L"CNpcTownDialog" == _strScriptName)
		return new CNpcTownDialog;
	if (L"CParticleLeaf" == _strScriptName)
		return new CParticleLeaf;
	if (L"CPauseFont" == _strScriptName)
		return new CPauseFont;
	if (L"CPlayButtonScript" == _strScriptName)
		return new CPlayButtonScript;
	if (L"CPlayerAttackScript" == _strScriptName)
		return new CPlayerAttackScript;
	if (L"CPlayerHit" == _strScriptName)
		return new CPlayerHit;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CWaterSplash" == _strScriptName)
		return new CWaterSplash;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BGBELLRING:
		return new CBGBellRing;
		break;
	case (UINT)SCRIPT_TYPE::BGMOVE:
		return new CBGMove;
		break;
	case (UINT)SCRIPT_TYPE::BOSS1HIT:
		return new CBoss1Hit;
		break;
	case (UINT)SCRIPT_TYPE::BOSS1HPCONTROLL:
		return new CBoss1HpControll;
		break;
	case (UINT)SCRIPT_TYPE::BOSS1HPEFFECT:
		return new CBoss1HpEffect;
		break;
	case (UINT)SCRIPT_TYPE::BOSS1SCRIPT:
		return new CBoss1Script;
		break;
	case (UINT)SCRIPT_TYPE::BOSS1SETTARGET:
		return new CBoss1SetTarget;
		break;
	case (UINT)SCRIPT_TYPE::BOSSSTAGESCRIPT:
		return new CBossStageScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERASCRIPT:
		return new CCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::CEILINGCHECK:
		return new CCeilingCheck;
		break;
	case (UINT)SCRIPT_TYPE::CHANGESTAGE:
		return new CChangeStage;
		break;
	case (UINT)SCRIPT_TYPE::COPYRIGHT:
		return new CCopyRight;
		break;
	case (UINT)SCRIPT_TYPE::DOROMOVE:
		return new CDoroMove;
		break;
	case (UINT)SCRIPT_TYPE::FONTPOS:
		return new CFontPos;
		break;
	case (UINT)SCRIPT_TYPE::GROUNDBOARD:
		return new CGroundBoard;
		break;
	case (UINT)SCRIPT_TYPE::GROUNDFIX:
		return new CGroundFix;
		break;
	case (UINT)SCRIPT_TYPE::GROUNDSCRIPT:
		return new CGroundScript;
		break;
	case (UINT)SCRIPT_TYPE::GROUNDSLOPE:
		return new CGroundSlope;
		break;
	case (UINT)SCRIPT_TYPE::GROUNDWALL:
		return new CGroundWall;
		break;
	case (UINT)SCRIPT_TYPE::HPCONTROLL:
		return new CHpControll;
		break;
	case (UINT)SCRIPT_TYPE::HPREDUCEFX:
		return new CHpReduceFx;
		break;
	case (UINT)SCRIPT_TYPE::HPRESTORE:
		return new CHpRestore;
		break;
	case (UINT)SCRIPT_TYPE::JELLYJUMP:
		return new CJellyJump;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTER1HIT:
		return new CMonster1Hit;
		break;
	case (UINT)SCRIPT_TYPE::MONSTER1SCRIPT:
		return new CMonster1Script;
		break;
	case (UINT)SCRIPT_TYPE::MONSTER2HITSCRIPT:
		return new CMonster2HitScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTER2SCRIPT:
		return new CMonster2Script;
		break;
	case (UINT)SCRIPT_TYPE::MONSTER2TARGET:
		return new CMonster2Target;
		break;
	case (UINT)SCRIPT_TYPE::MONSTER3HITSCRIPT:
		return new CMonster3HitScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTER3SCRIPT:
		return new CMonster3Script;
		break;
	case (UINT)SCRIPT_TYPE::MONSTER3SETTARGET:
		return new CMonster3SetTarget;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSETTARGET:
		return new CMonsterSetTarget;
		break;
	case (UINT)SCRIPT_TYPE::MPCONTROLL:
		return new CMpControll;
		break;
	case (UINT)SCRIPT_TYPE::NPCENTERDIALOG:
		return new CNpcEnterDialog;
		break;
	case (UINT)SCRIPT_TYPE::NPCTEMPLE:
		return new CNpcTemple;
		break;
	case (UINT)SCRIPT_TYPE::NPCTEMPLEDIALOG:
		return new CNpcTempleDialog;
		break;
	case (UINT)SCRIPT_TYPE::NPCTOWNDIALOG:
		return new CNpcTownDialog;
		break;
	case (UINT)SCRIPT_TYPE::PARTICLELEAF:
		return new CParticleLeaf;
		break;
	case (UINT)SCRIPT_TYPE::PAUSEFONT:
		return new CPauseFont;
		break;
	case (UINT)SCRIPT_TYPE::PLAYBUTTONSCRIPT:
		return new CPlayButtonScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERATTACKSCRIPT:
		return new CPlayerAttackScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERHIT:
		return new CPlayerHit;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::WATERSPLASH:
		return new CWaterSplash;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::BGBELLRING:
		return L"CBGBellRing";
		break;

	case SCRIPT_TYPE::BGMOVE:
		return L"CBGMove";
		break;

	case SCRIPT_TYPE::BOSS1HIT:
		return L"CBoss1Hit";
		break;

	case SCRIPT_TYPE::BOSS1HPCONTROLL:
		return L"CBoss1HpControll";
		break;

	case SCRIPT_TYPE::BOSS1HPEFFECT:
		return L"CBoss1HpEffect";
		break;

	case SCRIPT_TYPE::BOSS1SCRIPT:
		return L"CBoss1Script";
		break;

	case SCRIPT_TYPE::BOSS1SETTARGET:
		return L"CBoss1SetTarget";
		break;

	case SCRIPT_TYPE::BOSSSTAGESCRIPT:
		return L"CBossStageScript";
		break;

	case SCRIPT_TYPE::CAMERASCRIPT:
		return L"CCameraScript";
		break;

	case SCRIPT_TYPE::CEILINGCHECK:
		return L"CCeilingCheck";
		break;

	case SCRIPT_TYPE::CHANGESTAGE:
		return L"CChangeStage";
		break;

	case SCRIPT_TYPE::COPYRIGHT:
		return L"CCopyRight";
		break;

	case SCRIPT_TYPE::DOROMOVE:
		return L"CDoroMove";
		break;

	case SCRIPT_TYPE::FONTPOS:
		return L"CFontPos";
		break;

	case SCRIPT_TYPE::GROUNDBOARD:
		return L"CGroundBoard";
		break;

	case SCRIPT_TYPE::GROUNDFIX:
		return L"CGroundFix";
		break;

	case SCRIPT_TYPE::GROUNDSCRIPT:
		return L"CGroundScript";
		break;

	case SCRIPT_TYPE::GROUNDSLOPE:
		return L"CGroundSlope";
		break;

	case SCRIPT_TYPE::GROUNDWALL:
		return L"CGroundWall";
		break;

	case SCRIPT_TYPE::HPCONTROLL:
		return L"CHpControll";
		break;

	case SCRIPT_TYPE::HPREDUCEFX:
		return L"CHpReduceFx";
		break;

	case SCRIPT_TYPE::HPRESTORE:
		return L"CHpRestore";
		break;

	case SCRIPT_TYPE::JELLYJUMP:
		return L"CJellyJump";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTER1HIT:
		return L"CMonster1Hit";
		break;

	case SCRIPT_TYPE::MONSTER1SCRIPT:
		return L"CMonster1Script";
		break;

	case SCRIPT_TYPE::MONSTER2HITSCRIPT:
		return L"CMonster2HitScript";
		break;

	case SCRIPT_TYPE::MONSTER2SCRIPT:
		return L"CMonster2Script";
		break;

	case SCRIPT_TYPE::MONSTER2TARGET:
		return L"CMonster2Target";
		break;

	case SCRIPT_TYPE::MONSTER3HITSCRIPT:
		return L"CMonster3HitScript";
		break;

	case SCRIPT_TYPE::MONSTER3SCRIPT:
		return L"CMonster3Script";
		break;

	case SCRIPT_TYPE::MONSTER3SETTARGET:
		return L"CMonster3SetTarget";
		break;

	case SCRIPT_TYPE::MONSTERSETTARGET:
		return L"CMonsterSetTarget";
		break;

	case SCRIPT_TYPE::MPCONTROLL:
		return L"CMpControll";
		break;

	case SCRIPT_TYPE::NPCENTERDIALOG:
		return L"CNpcEnterDialog";
		break;

	case SCRIPT_TYPE::NPCTEMPLE:
		return L"CNpcTemple";
		break;

	case SCRIPT_TYPE::NPCTEMPLEDIALOG:
		return L"CNpcTempleDialog";
		break;

	case SCRIPT_TYPE::NPCTOWNDIALOG:
		return L"CNpcTownDialog";
		break;

	case SCRIPT_TYPE::PARTICLELEAF:
		return L"CParticleLeaf";
		break;

	case SCRIPT_TYPE::PAUSEFONT:
		return L"CPauseFont";
		break;

	case SCRIPT_TYPE::PLAYBUTTONSCRIPT:
		return L"CPlayButtonScript";
		break;

	case SCRIPT_TYPE::PLAYERATTACKSCRIPT:
		return L"CPlayerAttackScript";
		break;

	case SCRIPT_TYPE::PLAYERHIT:
		return L"CPlayerHit";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::WATERSPLASH:
		return L"CWaterSplash";
		break;

	}
	return nullptr;
}