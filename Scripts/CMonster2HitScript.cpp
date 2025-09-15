#include "pch.h"
#include "CMonster2HitScript.h"

#include "CMonster2Script.h"
#include "CPlayerHit.h"

#include <Engine/CSoundMgr.h>

CMonster2HitScript::CMonster2HitScript()
    : CScript((UINT)SCRIPT_TYPE::MONSTER2HITSCRIPT)
    , m_HitTime(0.f)
    , m_IsHit(false)
{
}

CMonster2HitScript::~CMonster2HitScript()
{
}

void CMonster2HitScript::SaveComponent(FILE* _File)
{
}

void CMonster2HitScript::LoadComponent(FILE* _File)
{
}

void CMonster2HitScript::Begin()
{
}

void CMonster2HitScript::Tick()
{
}

void CMonster2HitScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    if (_OtherObject->GetLayerIdx() == 12)
    {
        GetOwner()->GetParent()->FlipbookPlayer()->StartHitEffect(0.2f, Vec3(0.6f, 0.2f, 0.2f));
        GetOwner()->GetParent()->StateMachine()->ChangeState(L"CMonster2Hit");
        m_IsHit = true;

        const vector<CScript*>& vecScripts = GetOwner()->GetParent()->GetScripts();
        CMonster2Script* Monster1Script = (CMonster2Script*)vecScripts[0];
        Monster1Script->HitDamage(20);


        const vector<CScript*>& hitScript = _OtherObject->GetParent()->GetChild()[1]->GetScripts();
        CPlayerHit* pHit = (CPlayerHit*)hitScript[0];
        // MPÈ¸º¹
        pHit->UI_CalculateHPMP(10, false);
        CSoundMgr::GetInst()->PlayFX(L"Monster2Hit", 0.5f);
    }
}

void CMonster2HitScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CMonster2HitScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}