#include "pch.h"
#include "CMonsterSetTarget.h"
#include "CMonster1Script.h"



CMonsterSetTarget::CMonsterSetTarget()
    : CScript((UINT)SCRIPT_TYPE::MONSTERSETTARGET)
{
}

CMonsterSetTarget::~CMonsterSetTarget()
{
}

void CMonsterSetTarget::SaveComponent(FILE* _File)
{
}

void CMonsterSetTarget::LoadComponent(FILE* _File)
{
}

void CMonsterSetTarget::Begin()
{
}

void CMonsterSetTarget::Tick()
{
}

void CMonsterSetTarget::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    const vector<CScript*>& vecScripts = GetOwner()->GetParent()->GetScripts();
    CMonster1Script* MonsterScript1 = (CMonster1Script*)vecScripts[0];
    if (MonsterScript1 != nullptr)
    {
        MonsterScript1->SetTargetObject(_OtherObject);
        MonsterScript1->SetTrace(true);

    }

}

void CMonsterSetTarget::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{


}

void CMonsterSetTarget::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}