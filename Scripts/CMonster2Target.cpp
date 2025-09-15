#include "pch.h"
#include "CMonster2Target.h"

#include "CMonster2Script.h"

CMonster2Target::CMonster2Target()
    : CScript((UINT)SCRIPT_TYPE::MONSTER2TARGET)
{
}

CMonster2Target::~CMonster2Target()
{
}


void CMonster2Target::SaveComponent(FILE* _File)
{
}

void CMonster2Target::LoadComponent(FILE* _File)
{
}

void CMonster2Target::Begin()
{
}

void CMonster2Target::Tick()
{
}

void CMonster2Target::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    const vector<CScript*>& vecScripts = GetOwner()->GetParent()->GetScripts();

    CMonster2Script* MonsterScript2 = (CMonster2Script*)(vecScripts[0]);
    if (MonsterScript2 != nullptr)
    {
        MonsterScript2->SetTargetObject(_OtherObject);
        MonsterScript2->SetTrace(true);
        if(GetOwner()->GetParent()->RigidBody2D()->IsGround())
            GetOwner()->GetParent()->StateMachine()->ChangeState(L"CMonster2Attack");

    }
}

void CMonster2Target::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    const vector<CScript*>& vecScripts = GetOwner()->GetParent()->GetScripts();
    CMonster2Script* MonsterScript2 = (CMonster2Script*)(vecScripts[0]);
    
    if (MonsterScript2 != nullptr)
    {
        if (!(MonsterScript2->GetTrace()))
        {
            MonsterScript2->SetTargetObject(_OtherObject);
            MonsterScript2->SetTrace(true);
            if (GetOwner()->GetParent()->RigidBody2D()->IsGround())
                GetOwner()->GetParent()->StateMachine()->ChangeState(L"CMonster2Attack");
        }
    }

}

void CMonster2Target::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}
