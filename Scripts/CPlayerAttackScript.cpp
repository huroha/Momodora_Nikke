#include "pch.h"
#include "CPlayerAttackScript.h"

#include <Engine/CStateMachine.h>
#include <StateMachine/CPlayerAttack1.h>
#include <StateMachine/CPlayerAttack2.h>
#include <StateMachine/CPlayerAttack3.h>

#include <Engine/CSoundMgr.h>

CPlayerAttackScript::CPlayerAttackScript()
    : CScript((UINT)SCRIPT_TYPE::PLAYERATTACKSCRIPT)
{
}

CPlayerAttackScript::~CPlayerAttackScript()
{
}


void CPlayerAttackScript::SaveComponent(FILE* _File)
{
}

void CPlayerAttackScript::LoadComponent(FILE* _File)
{
}

void CPlayerAttackScript::Begin()
{
}

void CPlayerAttackScript::Tick()
{
}

void CPlayerAttackScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

    // 몬스터가 맞게되면 이동하면 안된다.
    if (_OtherObject->GetLayerIdx() == 14)
    {

        CState* CurState = GetOwner()->GetParent()->StateMachine()->GetCurState();
        if (CurState->GetName() == L"CPlayerAttack1")
        {
            CPlayerAttack1* AttackState = (CPlayerAttack1*)CurState;
            AttackState->SetAttackSuccess(true);
        }
        else if (CurState->GetName() == L"CPlayerAttack2")
        {
            CPlayerAttack2* AttackState = (CPlayerAttack2*)CurState;
            AttackState->SetAttackSuccess(true);
        }
        else if (CurState->GetName() == L"CPlayerAttack3")
        {
            CPlayerAttack3* AttackState = (CPlayerAttack3*)CurState;
        }
    }
}

void CPlayerAttackScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayerAttackScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}