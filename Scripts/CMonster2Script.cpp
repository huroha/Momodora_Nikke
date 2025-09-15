#include "pch.h"
#include "CMonster2Script.h"



CMonster2Script::CMonster2Script()
    : CScript((UINT)SCRIPT_TYPE::MONSTER2SCRIPT)
    , m_IsHit(false)
    , m_HP(60)
    , m_Dead(false)
{
}

CMonster2Script::~CMonster2Script()
{
}


void CMonster2Script::SaveComponent(FILE* _File)
{
}

void CMonster2Script::LoadComponent(FILE* _File)
{
}

void CMonster2Script::Begin()
{
    if (GetOwner()->GetComponent(COMPONENT_TYPE::STATEMACINE) != nullptr)
    {
        StateMachine()->ChangeState(L"CMonster2Idle");
    }
    if (GetOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D) != nullptr)
    {
        RigidBody2D()->SetMaxSpeed(750.f);
    }
}

void CMonster2Script::Tick()
{

    if (m_Target != nullptr && !m_IsHit)
    {
        // 플레이어가 오른쪽에 있으면, 오른쪽을 봐야함.
        
        if (m_Target->GetParent()->Transform()->GetRelativePos().x > GetOwner()->Transform()->GetRelativePos().x)
        {
            if (GetOwner()->Transform()->GetRelativeScale().x < 0)
            {
                if (GetOwner()->StateMachine()->GetCurStateName() != L"CMonster2Attack")
                {
                    GetOwner()->Transform()->SetRelativeScale(Vec3(160.f, 128.f, 1.f));
                    vector<CGameObject*> Child = GetOwner()->GetChild();
                    Child[1]->Collider2D()->SetOffset(Vec2(200.f, 0.f));

                }

            }
        }
        else
        {
            if (GetOwner()->Transform()->GetRelativeScale().x > 0)
            {
                if (GetOwner()->StateMachine()->GetCurStateName() != L"CMonster2Attack")
                {
                    GetOwner()->Transform()->SetRelativeScale(Vec3(-160.f, 128.f, 1.f));
                    vector<CGameObject*> Child = GetOwner()->GetChild();
                    Child[1]->Collider2D()->SetOffset(Vec2(-200.f, 0.f));
                }
            }
        }
        m_Time += DT;
    }

    // 공격 재시도 진입전
    if (m_Time > 1.5f && !m_Dead)
    {
        m_Time = 0;
        StateMachine()->ChangeState(L"CMonster2Idle");
    }

}

void CMonster2Script::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CMonster2Script::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CMonster2Script::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}