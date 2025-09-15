#include "pch.h"
#include "CGroundSlope.h"
#include "CPlayerScript.h"



CGroundSlope::CGroundSlope()
    : CScript((UINT)SCRIPT_TYPE::GROUNDSLOPE)
    , m_IsRight(false)
    , m_SlopeAngle(0.f)

{
}

CGroundSlope::~CGroundSlope()
{
}



void CGroundSlope::Begin()
{
    // 경사면의 각도는 Transform의 회전값과 동기화
    Vec3 rot = Transform()->GetRelativeRotation();
    SetSlopeAngle(rot.z);


}

void CGroundSlope::Tick()
{

}


void CGroundSlope::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    if (_OtherObject->GetLayerIdx() == 15)
    {
        CPlayerScript* pPlayerScript = (CPlayerScript*)_OtherObject->GetParent()->GetScripts()[0];
        if (pPlayerScript)
        {
            pPlayerScript->SetSlopeAngle(m_SlopeAngle);  // 각도 정보만 전달
            _OtherObject->GetParent()->RigidBody2D()->SetGround(true);
        }
    }

}

void CGroundSlope::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

}

void CGroundSlope::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

}


void CGroundSlope::SaveComponent(FILE* _File)
{
}

void CGroundSlope::LoadComponent(FILE* _File)
{
}