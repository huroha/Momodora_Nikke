#include "pch.h"
#include "CDoroMove.h"




CDoroMove::CDoroMove()
    : CScript((UINT)SCRIPT_TYPE::DOROMOVE)
{
}

CDoroMove::~CDoroMove()
{
}

void CDoroMove::SaveComponent(FILE* _File)
{
}

void CDoroMove::LoadComponent(FILE* _File)
{
}

void CDoroMove::Begin()
{
}

void CDoroMove::Tick()
{

}

void CDoroMove::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CDoroMove::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    if (RigidBody2D())
    {
        RigidBody2D()->AddForce(Vec3(50.f, 0.f, 0.f));
    }
}

void CDoroMove::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    if (RigidBody2D())
    {
        RigidBody2D()->SetVelocity(Vec3(0.f, 0.f, 0.f));
        RigidBody2D()->UseGravity(false);
    }
}