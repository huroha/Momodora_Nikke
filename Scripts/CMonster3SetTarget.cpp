#include "pch.h"
#include "CMonster3SetTarget.h"

#include "CMonster3Script.h"

CMonster3SetTarget::CMonster3SetTarget()
    : CScript((UINT)SCRIPT_TYPE::MONSTER3SETTARGET)
{
}

CMonster3SetTarget::~CMonster3SetTarget()
{
}

void CMonster3SetTarget::SaveComponent(FILE* _File)
{
}

void CMonster3SetTarget::LoadComponent(FILE* _File)
{
}

void CMonster3SetTarget::Begin()
{
}

void CMonster3SetTarget::Tick()
{
}

void CMonster3SetTarget::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    const vector<CScript*>& vecScripts = GetOwner()->GetParent()->GetScripts();
    CMonster3Script* MonsterScript3 = (CMonster3Script*)vecScripts[0];
    if (MonsterScript3 != nullptr)
    {
        MonsterScript3->SetTargetObject(_OtherObject);
        MonsterScript3->SetTrace(true);

    }
}

void CMonster3SetTarget::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

}

void CMonster3SetTarget::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    const vector<CScript*>& vecScripts = GetOwner()->GetParent()->GetScripts();
    CMonster3Script* MonsterScript3 = (CMonster3Script*)vecScripts[0];
    if (MonsterScript3 != nullptr)
    {
        MonsterScript3->SetTargetObject(nullptr);
        MonsterScript3->SetTrace(false);
    }
}