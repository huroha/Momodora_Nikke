#include "pch.h"
#include "CCeilingCheck.h"


CCeilingCheck::CCeilingCheck()
    :CScript((UINT)SCRIPT_TYPE::CEILINGCHECK)
{
}

CCeilingCheck::~CCeilingCheck()
{
}


void CCeilingCheck::SaveComponent(FILE* _File)
{
}

void CCeilingCheck::LoadComponent(FILE* _File)
{
}

void CCeilingCheck::Begin()
{
}

void CCeilingCheck::Tick()
{
}

void CCeilingCheck::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    // Player Collider�� �΋H���� jumpDown���� �ٲپ�� ��.
    CGameObject* pOtherObj = _OtherObject;
    if (pOtherObj->GetLayerIdx() == 3)
    {
        pOtherObj->GetParent()->RigidBody2D()->SetGravityVelocity(-1.f);
    }
}

void CCeilingCheck::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CCeilingCheck::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}
