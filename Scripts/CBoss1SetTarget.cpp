#include "pch.h"
#include "CBoss1SetTarget.h"
#include "CBoss1Script.h"


CBoss1SetTarget::CBoss1SetTarget()
    : CScript((UINT)SCRIPT_TYPE::BOSS1SETTARGET)
    , m_LookOn(false)
    , m_Once(false)
{
}

CBoss1SetTarget::~CBoss1SetTarget()
{
}

void CBoss1SetTarget::SaveComponent(FILE* _File)
{
}

void CBoss1SetTarget::LoadComponent(FILE* _File)
{
}

void CBoss1SetTarget::Begin()
{
}

void CBoss1SetTarget::Tick()
{
    if (!m_Once && m_LookOn)
    {
        GetOwner()->GetParent()->StateMachine()->ChangeState(L"CBoss1Idle2");
        m_Once = true;
    }

}

void CBoss1SetTarget::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    const vector<CScript*>& vecScripts = GetOwner()->GetParent()->GetScripts();
    CBoss1Script* BossScript1 = dynamic_cast<CBoss1Script*>(vecScripts[0]);
    if (BossScript1 != nullptr)
        BossScript1->SetTargetObject(_OtherObject);

    m_LookOn = true;
}

void CBoss1SetTarget::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss1SetTarget::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}
