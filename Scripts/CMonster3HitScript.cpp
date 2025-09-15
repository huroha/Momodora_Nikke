#include "pch.h"
#include "CMonster3HitScript.h"
#include "CMonster3Script.h"
#include "CPlayerHit.h"
#include <Engine/CSoundMgr.h>

CMonster3HitScript::CMonster3HitScript()
    : CScript((UINT)SCRIPT_TYPE::MONSTER3HITSCRIPT)
    , m_HitTime(0.f)
    , m_IsHit(false)
{
}

CMonster3HitScript::~CMonster3HitScript()
{
}

void CMonster3HitScript::SaveComponent(FILE* _File)
{
}

void CMonster3HitScript::LoadComponent(FILE* _File)
{
}

void CMonster3HitScript::Begin()
{
}

void CMonster3HitScript::Tick()
{
}

void CMonster3HitScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    if (_OtherObject->GetLayerIdx() == 12)
    {
        GetOwner()->GetParent()->FlipbookPlayer()->StartHitEffect(0.2f, Vec3(0.6f, 0.2f, 0.2f));
        GetOwner()->GetParent()->StateMachine()->ChangeState(L"CMonster3Hit");
        m_IsHit = true;

        const vector<CScript*>& vecScripts = GetOwner()->GetParent()->GetScripts();
        CMonster3Script* Monster3Script = (CMonster3Script*)vecScripts[0];
        Monster3Script->HitDamage(20);


        const vector<CScript*>& hitScript = _OtherObject->GetParent()->GetChild()[1]->GetScripts();
        CPlayerHit* pHit = (CPlayerHit*)hitScript[0];
        // MPÈ¸º¹
        pHit->UI_CalculateHPMP(10, false);

        CSoundMgr::GetInst()->PlayFX(L"Monster3Hit", 0.5f);
    }
}

void CMonster3HitScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CMonster3HitScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}
