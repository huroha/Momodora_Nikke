#include "pch.h"
#include "CGroundWall.h"



CGroundWall::CGroundWall()
    : CScript((UINT)SCRIPT_TYPE::GROUNDWALL)
    , m_IsCollision(false)
{
}

CGroundWall::~CGroundWall()
{
}

void CGroundWall::SaveComponent(FILE* _File)
{
}

void CGroundWall::LoadComponent(FILE* _File)
{
}

void CGroundWall::Begin()
{
}

void CGroundWall::Tick()
{
}

void CGroundWall::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    CGameObject* pObject = _OtherObject;

    if (pObject->GetLayerIdx() != 1)
        return;
    m_IsCollision = true;
    

    // ���� �浹���� ���
    float wallPosX = Transform()->GetRelativePos().x;
    float wallScaleX = _Collider->GetScale().x / 2;
    float objectScaleX = _OtherCollider->GetScale().x / 2;
    Vec3 objPos = pObject->Transform()->GetRelativePos();

    // ���� ������ ������ ������ Ȯ�� �� ��ġ ����
    if (objPos.x < wallPosX)
    {
        // ���� ��
        pObject->Transform()->SetRelativePos(Vec3(wallPosX - wallScaleX - objectScaleX, objPos.y, objPos.z));
    }
    else
    {
        // ������ ��
        pObject->Transform()->SetRelativePos(Vec3(wallPosX + wallScaleX + objectScaleX, objPos.y, objPos.z));
    }

    // ���� �̵� ����
    if (pObject->RigidBody2D())
    {
        Vec3 velocity = pObject->RigidBody2D()->GetVelocity();

        // �� ���������� �ӵ��� 0���� ����
        if (objPos.x < wallPosX && velocity.x < 0.f)  // ���� ���� �� �������� ������ ���
        {
            velocity.x = 0.f;
        }
        else if (objPos.x > wallPosX && velocity.x > 0.f)  // ������ ���� �� ���������� ������ ���
        {
            velocity.x = 0.f;
        }

        pObject->RigidBody2D()->SetVelocity(velocity);
    }

}

void CGroundWall::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    CGameObject* pObject = _OtherObject;

    if (pObject->GetLayerIdx() != 1)
        return;
    float wallPosX = Transform()->GetRelativePos().x;
    float wallScaleX = _Collider->GetScale().x / 2;
    float objectScaleX = _OtherCollider->GetScale().x / 2;
    Vec3 objPos = pObject->Transform()->GetRelativePos();
    if (m_IsCollision)
    {

        // ���� �浹 ���� ��� ��� ó��

        // ���� ������ ������ ������ Ȯ�� �� ��ġ ����
        if (objPos.x < wallPosX)
        {
            // ���� ��
            pObject->Transform()->SetRelativePos(Vec3(wallPosX - wallScaleX - objectScaleX, objPos.y, objPos.z));
        }
        else
        {
            // ������ ��
            pObject->Transform()->SetRelativePos(Vec3(wallPosX + wallScaleX + objectScaleX, objPos.y, objPos.z));
        }

        // ���� �̵� ����
        if (pObject->RigidBody2D())
        {
            Vec3 velocity = pObject->RigidBody2D()->GetVelocity();

            // �� ���������� �ӵ��� 0���� ����
            if (objPos.x < wallPosX && velocity.x < 0.f)  // ���� ���� �� �������� ������ ���
            {
                velocity.x = 0.f;
            }
            else if (objPos.x > wallPosX && velocity.x > 0.f)  // ������ ���� �� ���������� ������ ���
            {
                velocity.x = 0.f;
            }

            pObject->RigidBody2D()->SetVelocity(velocity);
        }
    }

    // ���ʿ��� �浹
    if (objPos.x < wallPosX)
    {
        if (pObject->StateMachine()->GetCurStateName() != L"CPlayerRun")
        {
            if (KEY_PRESSED(KEY::LEFT))
            {
                m_IsCollision = false;
                pObject->RigidBody2D()->SetVelocity(Vec3(-30.f,0.f,0.f));
            }
        }
    }
    else if (objPos.x > wallPosX)
    {
        if (pObject->StateMachine()->GetCurStateName() != L"CPlayerRun")
        {
            if (KEY_PRESSED(KEY::RIGHT))
            {
                m_IsCollision = false;
                pObject->RigidBody2D()->SetVelocity(Vec3(30.f, 0.f, 0.f));
            }
        }
    }
    
    
}

void CGroundWall::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

}
