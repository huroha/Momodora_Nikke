#include "pch.h"
#include "CMonster1Script.h"



CMonster1Script::CMonster1Script()
    : CScript((UINT)SCRIPT_TYPE::MONSTER1SCRIPT)
    , m_Target(nullptr)
    , m_Time(0.f)
    , m_HP(100)
{
}

CMonster1Script::~CMonster1Script()
{
}

void CMonster1Script::SaveComponent(FILE* _File)
{
}

void CMonster1Script::LoadComponent(FILE* _File)
{
}

void CMonster1Script::Begin()
{
    if (GetOwner()->GetComponent(COMPONENT_TYPE::STATEMACINE) != nullptr)
    {
        StateMachine()->ChangeState(L"CMonsterIdle");
    }
    if (GetOwner()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D) != nullptr)
    {
        RigidBody2D()->SetMaxSpeed(200.f);
    }
}

void CMonster1Script::Tick()
{
    if (m_Trace)
    {
        if (m_Target != nullptr)
        {
            // 플레이어가 오른쪽에 있으면, 오른쪽을 봐야함.
            if (m_Target->GetParent()->Transform()->GetRelativePos().x > GetOwner()->Transform()->GetRelativePos().x)
            {
                if (GetOwner()->Transform()->GetRelativeScale().x < 0)
                {
                    if (GetOwner()->StateMachine()->GetCurStateName() != L"CMonsterAttack")
                    {
                        GetOwner()->Transform()->SetRelativeScale(Vec3(240.f, 130.f, 1.f));
                        vector<CGameObject*> Child = GetOwner()->GetChild();
                        Child[1]->Collider2D()->SetOffset(Vec2(250.f, 0.f));
                        Child[2]->Collider2D()->SetOffset(Vec2(30.f, 0.f));
                    }

                }
            }
            else
            {
                if (GetOwner()->Transform()->GetRelativeScale().x > 0)
                {
                    if (GetOwner()->StateMachine()->GetCurStateName() != L"CMonsterAttack")
                    {
                        GetOwner()->Transform()->SetRelativeScale(Vec3(-240.f, 130.f, 1.f));
                        vector<CGameObject*> Child = GetOwner()->GetChild();
                        Child[1]->Collider2D()->SetOffset(Vec2(-250.f, 0.f));
                        Child[2]->Collider2D()->SetOffset(Vec2(-30.f, 0.f));
                    }
                }
            }
            m_Time += DT;
        }
    }

    // 추적 포기
    if (m_Time > 30.f)
    {
        m_Trace = false;
        m_Time = 0;
        m_Target = nullptr;
        StateMachine()->ChangeState(L"CMonsterIdle");
    }

}

void CMonster1Script::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{


}

void CMonster1Script::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CMonster1Script::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}