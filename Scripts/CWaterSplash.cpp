#include "pch.h"
#include "CWaterSplash.h"

#include "Engine/CSoundMgr.h"

CWaterSplash::CWaterSplash()
    : CScript((UINT)SCRIPT_TYPE::WATERSPLASH)
{
}

CWaterSplash::~CWaterSplash()
{
}

void CWaterSplash::SaveComponent(FILE* _File)
{
}

void CWaterSplash::LoadComponent(FILE* _File)
{
}

void CWaterSplash::Begin()
{
}

void CWaterSplash::Tick()
{
}

void CWaterSplash::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    CGameObject* WaterSplash = GetOwner()->GetChild()[0];
    float xPos = Transform()->GetRelativePos().x;
    float playerPos = _OtherObject->Transform()->GetRelativePos().x;
    WaterSplash->Transform()->SetRelativePos(Vec3(playerPos - xPos, 53.f, 0.f));
    WaterSplash->FlipbookPlayer()->Play(0, 12, false);
    CSoundMgr::GetInst()->PlayFX(L"WaterPond", 0.7f);
}

void CWaterSplash::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CWaterSplash::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    CGameObject* WaterSplash = GetOwner()->GetChild()[1];
    float xPos = Transform()->GetRelativePos().x;
    float playerPos = _OtherObject->Transform()->GetRelativePos().x;
    WaterSplash->Transform()->SetRelativePos(Vec3(playerPos - xPos, 53.f, 0.f));
    WaterSplash->FlipbookPlayer()->Play(0, 12, false);
    CSoundMgr::GetInst()->PlayFX(L"WaterPond", 0.7f);
}

