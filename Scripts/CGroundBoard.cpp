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

    // Player collider position�� ������ ���ؼ�, �Ʒ��� ���� ������ ó������.
    CGameObject* pObj = _OtherObject;

    if (pObj->GetLayerIdx() != 1)
        return;
    float pColliderPos = pObj->Transform()->GetRelativePos().y + _OtherCollider->GetOffset().y+3;
    float BoardPos =Transform()->GetRelativePos().y;

    float BoarduPos = Transform()->GetRelativePos().y + (Collider2D()->GetScale().y / 2) + Collider2D()->GetOffset().y;

    // �Ʒ����� ���� �����Ҷ� �� ���
    if (pColliderPos < BoardPos)
        return;

    // ������ �浹�ϸ� Ground Check
    else if (pColliderPos > BoarduPos)
    {
        // ������ ��ġ�� �����ؾ���.
        float Scale = _OtherCollider->GetScale().y / 2;
        float Offset = _OtherCollider->GetOffset().y;

        pObj->RigidBody2D()->SetGround(true);
        
        // ������ Slope ó���� �Ǿ������� ������ 0���� �ʱ�ȭ���ִ� ���� �߰�
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
