#include "pch.h"
#include "CMonster1Hit.h"

#include "CMonster1Script.h"
#include "CPlayerHit.h"
#include <Engine/CLevelMgr.h>
#include "CCameraScript.h"
#include "CPlayerScript.h"
#include <cstdlib> 
#include <ctime>   

CMonster1Hit::CMonster1Hit()
    : CScript((UINT)SCRIPT_TYPE::MONSTER1HIT)
{
}

CMonster1Hit::~CMonster1Hit()
{
}

void CMonster1Hit::SaveComponent(FILE* _File)
{
}

void CMonster1Hit::LoadComponent(FILE* _File)
{
}

void CMonster1Hit::Begin()
{
    srand(time(nullptr));
}

void CMonster1Hit::Tick()
{

}

void CMonster1Hit::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    if (_OtherObject->GetLayerIdx() == 12)
    {
        GetOwner()->GetParent()->FlipbookPlayer()->StartHitEffect(0.2f, Vec3(0.6f, 0.2f, 0.2f));
        GetOwner()->GetParent()->StateMachine()->ChangeState(L"CMonsterHit");
        m_IsHit = true;

        const vector<CScript*>& vecScripts = GetOwner()->GetParent()->GetScripts();
        CMonster1Script* Monster1Script = (CMonster1Script*)vecScripts[0];
        Monster1Script->HitDamage(20);


        const vector<CScript*>& hitScript = _OtherObject->GetParent()->GetChild()[1]->GetScripts();
        CPlayerHit* pHit = (CPlayerHit*)hitScript[0];
        pHit->UI_CalculateHPMP(10, false);


        CGameObject* hitFx = GetOwner()->GetParent()->GetChild()[4];
        int random = rand() % 3;
        if (GetOwner()->GetParent()->Transform()->GetRelativeScale().x > 0)
        {
            hitFx->Transform()->SetRelativePos(Vec3(-20.f, -10.f, 0.f));
            hitFx->Transform()->SetRelativeScale(Vec3(48.f, 64.f, 1.f));
        }
        else
        {
            hitFx->Transform()->SetRelativePos(Vec3(20.f, -10.f, 0.f));
            hitFx->Transform()->SetRelativeScale(Vec3(-48.f, 64.f, 1.f));
        }
        hitFx->FlipbookPlayer()->Play(random, 14, false);


    }

}

void CMonster1Hit::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CMonster1Hit::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    if (_OtherObject->GetLayerIdx() == 12)  // 플레이어 공격 레이어인 경우
    {
        m_IsHit = false;  // hit 상태 해제
        m_HitTime = 0.f;  // 타이머도 리셋
    }
}