#include "pch.h"
#include "CJellyJump.h"
#include "CPlayerScript.h"

#include <Engine/CSoundMgr.h>


CJellyJump::CJellyJump()
    : CScript((UINT)SCRIPT_TYPE::JELLYJUMP)
    , m_JumpSpeed(0.f)
{
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Jump Speed", &m_JumpSpeed });
}

CJellyJump::~CJellyJump()
{
}

void CJellyJump::SaveComponent(FILE* _File)
{
    fwrite(&m_JumpSpeed, sizeof(float), 1, _File);
}

void CJellyJump::LoadComponent(FILE* _File)
{
    fread(&m_JumpSpeed, sizeof(float), 1, _File);
}

void CJellyJump::Begin()
{
}

void CJellyJump::Tick()
{
}

void CJellyJump::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    CGameObject* pOtherObj = _OtherObject;
    if (pOtherObj->GetLayerIdx() == 1)
    {
        pOtherObj->RigidBody2D()->JellyJump(m_JumpSpeed);
        pOtherObj->StateMachine()->ChangeState(L"CPlayerJumpUp");

        CSoundMgr::GetInst()->PlayFX(L"JellyJump", 1.f);
    }

    
}

void CJellyJump::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CJellyJump::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}
