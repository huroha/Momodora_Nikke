#include "pch.h"
#include "CGroundScript.h"
#include "CPlayerScript.h"

CGroundScript::CGroundScript()
    :CScript((UINT)SCRIPT_TYPE::GROUNDSCRIPT)
{
}

CGroundScript::~CGroundScript()
{
}

void CGroundScript::Begin()
{
}

void CGroundScript::Tick()
{
}

void CGroundScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    // 상대방의 위치를 조정해야함.
    
    
    CGameObject* pObject = _OtherObject;

    // Monster_Body 예외 추가
    if (pObject->GetLayerIdx() == 6)
        return;

    float Scale = _OtherCollider->GetScale().y/2;
    float Offset = _OtherCollider->GetOffset().y;

    if (pObject->RigidBody2D())
    {
        pObject->RigidBody2D()->SetGround(true);

        float LandPos = Transform()->GetRelativePos().y;
        float LandScale = _Collider->GetScale().y / 2;
        Vec3 ObjPos = pObject->Transform()->GetRelativePos();
        pObject->Transform()->SetRelativePos(Vec3(ObjPos.x, LandPos + Scale - Offset + LandScale, ObjPos.z));

        if (pObject->GetLayerIdx() == 1)
        {
            CPlayerScript* pPlayerScript = (CPlayerScript*)_OtherObject->GetScripts()[0];
            pPlayerScript->SetSlopeAngle(0.f);
        }

    }
    
}

void CGroundScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

}

void CGroundScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    CGameObject* pObject = _OtherObject;
    if (pObject->RigidBody2D() && pObject->GetLayerIdx() ==1)
    {
        CPlayerScript* pPlayerScript = (CPlayerScript*)_OtherObject->GetScripts()[0];
        if(pPlayerScript->GetSlopeAngle() == 0)
            pObject->RigidBody2D()->SetGround(false);
    }
    else if(pObject->RigidBody2D())
    {
        pObject->RigidBody2D()->SetGround(false);
    }
}

void CGroundScript::SaveComponent(FILE* _File)
{
}

void CGroundScript::LoadComponent(FILE* _File)
{
}
