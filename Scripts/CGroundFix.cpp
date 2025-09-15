#include "pch.h"
#include "CGroundFix.h"



CGroundFix::CGroundFix()
    :CScript((UINT)SCRIPT_TYPE::GROUNDFIX)
{
}

CGroundFix::~CGroundFix()
{
}


void CGroundFix::SaveComponent(FILE* _File)
{
}

void CGroundFix::LoadComponent(FILE* _File)
{
}

void CGroundFix::Begin()
{
}

void CGroundFix::Tick()
{
}

void CGroundFix::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    CGameObject* pObj = _OtherObject;
    if (pObj->GetLayerIdx() != 1)
        return;

    // 공중에 뜨는 부분 따로 막아줘야함...
    if (pObj->RigidBody2D()->IsGround() != false)
    {
        pObj->RigidBody2D()->SetGround(false);
    }

}

void CGroundFix::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CGroundFix::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}