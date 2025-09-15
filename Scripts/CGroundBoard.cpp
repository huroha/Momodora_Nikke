#include "pch.h"
#include "CGroundBoard.h"
#include "CPlayerScript.h"


CGroundBoard::CGroundBoard()
    :CScript((UINT)SCRIPT_TYPE::GROUNDBOARD)
    , m_OtherObject(nullptr)
    , m_IsCollision(false)
{
}

CGroundBoard::~CGroundBoard()
{
}

void CGroundBoard::SaveComponent(FILE* _File)
{
}

void CGroundBoard::LoadComponent(FILE* _File)
{
}

void CGroundBoard::Begin()
{
}

void CGroundBoard::Tick()
{
    if (m_IsCollision)
    {
        if (KEY_PRESSED(KEY::DOWN))
        {
            if (KEY_TAP(KEY::SPACE) && m_OtherObject->GetName() == L"Player")
            {
                m_OtherObject->RigidBody2D()->SetGround(false);
                m_OtherObject->StateMachine()->ChangeState(L"CPlayerJumpDown");
            }
        }
    }

}


void CGroundBoard::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

    // Player collider position을 가져와 비교해서, 아래와 위로 나누어 처리하자.
    CGameObject* pObj = _OtherObject;

    if (pObj->GetLayerIdx() != 1)
        return;
    float pColliderPos = pObj->Transform()->GetRelativePos().y + _OtherCollider->GetOffset().y+3;
    float BoardPos =Transform()->GetRelativePos().y;

    float BoarduPos = Transform()->GetRelativePos().y + (Collider2D()->GetScale().y / 2) + Collider2D()->GetOffset().y;

    // 아래에서 위로 점프할땐 걍 통과
    if (pColliderPos < BoardPos)
        return;

    // 위에서 충돌하면 Ground Check
    else if (pColliderPos > BoarduPos)
    {
        // 상대방의 위치를 조정해야함.
        float Scale = _OtherCollider->GetScale().y / 2;
        float Offset = _OtherCollider->GetOffset().y;

        pObj->RigidBody2D()->SetGround(true);
        
        // 이전에 Slope 처리가 되어있으면 강제로 0으로 초기화해주는 구문 추가
        CPlayerScript* pPlayerScript = (CPlayerScript*)pObj->GetScripts()[0];
        pPlayerScript->SetSlopeAngle(0);

        float LandPos = Transform()->GetRelativePos().y;
        float LandScale = _Collider->GetScale().y / 2;
        float BoardOffset = _Collider->GetOffset().y;
        Vec3 ObjPos = pObj->Transform()->GetRelativePos();
        pObj->Transform()->SetRelativePos(Vec3(ObjPos.x, LandPos + Scale - Offset + LandScale+ BoardOffset, ObjPos.z));
        m_IsCollision = true;
        m_OtherObject = _OtherObject;
        if(pObj->RigidBody2D()->GetVelocity().x == 0)
            pObj->StateMachine()->ChangeState(L"CPlayerLand");
    }

}

void CGroundBoard::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

}

void CGroundBoard::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    CGameObject* pObject = _OtherObject;

    if (pObject->GetLayerIdx() != 1)
        return;
    pObject->RigidBody2D()->SetGround(false);
    m_IsCollision = false;
    m_OtherObject = nullptr;
}
