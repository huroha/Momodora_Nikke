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
    

    // 벽과 충돌했을 경우
    float wallPosX = Transform()->GetRelativePos().x;
    float wallScaleX = _Collider->GetScale().x / 2;
    float objectScaleX = _OtherCollider->GetScale().x / 2;
    Vec3 objPos = pObject->Transform()->GetRelativePos();

    // 왼쪽 벽인지 오른쪽 벽인지 확인 및 위치 조정
    if (objPos.x < wallPosX)
    {
        // 왼쪽 벽
        pObject->Transform()->SetRelativePos(Vec3(wallPosX - wallScaleX - objectScaleX, objPos.y, objPos.z));
    }
    else
    {
        // 오른쪽 벽
        pObject->Transform()->SetRelativePos(Vec3(wallPosX + wallScaleX + objectScaleX, objPos.y, objPos.z));
    }

    // 수평 이동 제한
    if (pObject->RigidBody2D())
    {
        Vec3 velocity = pObject->RigidBody2D()->GetVelocity();

        // 벽 방향으로의 속도만 0으로 설정
        if (objPos.x < wallPosX && velocity.x < 0.f)  // 왼쪽 벽일 때 왼쪽으로 가려는 경우
        {
            velocity.x = 0.f;
        }
        else if (objPos.x > wallPosX && velocity.x > 0.f)  // 오른쪽 벽일 때 오른쪽으로 가려는 경우
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

        // 벽과 충돌 중일 경우 계속 처리

        // 왼쪽 벽인지 오른쪽 벽인지 확인 및 위치 조정
        if (objPos.x < wallPosX)
        {
            // 왼쪽 벽
            pObject->Transform()->SetRelativePos(Vec3(wallPosX - wallScaleX - objectScaleX, objPos.y, objPos.z));
        }
        else
        {
            // 오른쪽 벽
            pObject->Transform()->SetRelativePos(Vec3(wallPosX + wallScaleX + objectScaleX, objPos.y, objPos.z));
        }

        // 수평 이동 제한
        if (pObject->RigidBody2D())
        {
            Vec3 velocity = pObject->RigidBody2D()->GetVelocity();

            // 벽 방향으로의 속도만 0으로 설정
            if (objPos.x < wallPosX && velocity.x < 0.f)  // 왼쪽 벽일 때 왼쪽으로 가려는 경우
            {
                velocity.x = 0.f;
            }
            else if (objPos.x > wallPosX && velocity.x > 0.f)  // 오른쪽 벽일 때 오른쪽으로 가려는 경우
            {
                velocity.x = 0.f;
            }

            pObject->RigidBody2D()->SetVelocity(velocity);
        }
    }

    // 왼쪽에서 충돌
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
