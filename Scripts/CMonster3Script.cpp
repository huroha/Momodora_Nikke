#include "pch.h"
#include "CMonster3Script.h"




CMonster3Script::CMonster3Script()
    : CScript((UINT)SCRIPT_TYPE::MONSTER3SCRIPT)
    , m_Target(nullptr)
    , m_Time(0.f)
    , m_Trace(false)
    , m_IsHit(false)
    , m_HP(60)
    , m_Dead(false)
{
}

CMonster3Script::~CMonster3Script()
{
}

void CMonster3Script::SaveComponent(FILE* _File)
{
}

void CMonster3Script::LoadComponent(FILE* _File)
{
}

void CMonster3Script::Begin()
{
    if (GetOwner()->GetComponent(COMPONENT_TYPE::STATEMACINE) != nullptr)
    {
        StateMachine()->ChangeState(L"CMonster3Idle");
    }
    GetOwner()->GetChild()[2]->MeshRender()->SetActive(false);
}

void CMonster3Script::Tick()
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
                    if (GetOwner()->StateMachine()->GetCurStateName() != L"CMonster3Attack")
                    {
                        GetOwner()->Transform()->SetRelativeScale(Vec3(64.f, 64.f, 1.f));
                        vector<CGameObject*> Child = GetOwner()->GetChild();
                        Child[1]->Collider2D()->SetOffset(Vec2(180.f, 0.f));
                    }

                }
            }
            else
            {
                if (GetOwner()->Transform()->GetRelativeScale().x > 0)
                {
                    if (GetOwner()->StateMachine()->GetCurStateName() != L"CMonster3Attack")
                    {
                        GetOwner()->Transform()->SetRelativeScale(Vec3(-64.f, 64.f, 1.f));
                        vector<CGameObject*> Child = GetOwner()->GetChild();
                        Child[1]->Collider2D()->SetOffset(Vec2(-180.f, 0.f));
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
        StateMachine()->ChangeState(L"CMonster3Idle");
    }
}

void CMonster3Script::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CMonster3Script::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CMonster3Script::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}
