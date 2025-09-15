#include "pch.h"
#include "CBoss1Hit.h"

#include "CBoss1Script.h"
#include "CPlayerHit.h"
#include <Engine/CLevelMgr.h>
#include "CBoss1HpControll.h"
#include "CBoss1HpEffect.h"

CBoss1Hit::CBoss1Hit()
    : CScript((UINT)SCRIPT_TYPE::BOSS1HIT)
    , m_HpRemain(nullptr)
    , m_HitTime(0.f)
    , m_IsHit(false)
{
    
}

CBoss1Hit::~CBoss1Hit()
{
}

void CBoss1Hit::SaveComponent(FILE* _File)
{
}

void CBoss1Hit::LoadComponent(FILE* _File)
{
}

void CBoss1Hit::Begin()
{
    m_HpRemain = CLevelMgr::GetInst()->FindObjectByName(L"Boss_Remain");
    m_HpRemain2 = CLevelMgr::GetInst()->FindObjectByName(L"Boss_Remainfx");
}

void CBoss1Hit::Tick()
{


}

void CBoss1Hit::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    if (_OtherObject->GetLayerIdx() == 12)
    {
        GetOwner()->GetParent()->FlipbookPlayer()->StartHitEffect(0.2f, Vec3(0.4f, 0.4f, 0.4f));
        m_IsHit = true;

        const vector<CScript*>& vecScripts = GetOwner()->GetParent()->GetScripts();
        CBoss1Script* Monster1Script = (CBoss1Script*)vecScripts[0];
      
 


        const vector<CScript*>& hitScript = _OtherObject->GetParent()->GetChild()[1]->GetScripts();
        CPlayerHit* pHit = (CPlayerHit*)hitScript[0];
        pHit->UI_CalculateHPMP(10, false);
        if(pHit->IsBust())
            Monster1Script->HitDamage(30);
        else
            Monster1Script->HitDamage(20);

        int RemainHp = Monster1Script->GetMonsterHP();
        ((CBoss1HpControll*)m_HpRemain->GetScripts()[0])->SettingHp(RemainHp);
        ((CBoss1HpEffect*)m_HpRemain2->GetScripts()[0])->SettingHp(RemainHp);

        CGameObject* hitFx = GetOwner()->GetParent()->GetChild()[5];
        int random = rand() % 3;
        Vec3 PlayerPos = _OtherObject->GetParent()->Transform()->GetRelativePos();
        Vec3 BossPos = GetOwner()->GetParent()->Transform()->GetRelativePos();
        if (PlayerPos.x < BossPos.x)
        {
            //플레이어가 왼쪽에서 쳤음
            hitFx->Transform()->SetRelativePos(Vec3(PlayerPos.x - BossPos.x + _OtherCollider->GetScale().x, PlayerPos.y - BossPos.y, -10.f));
            hitFx->Transform()->SetRelativeScale(Vec3(-95.f, 134.f, 1.f));
        }
        else
        {

            hitFx->Transform()->SetRelativePos(Vec3(PlayerPos.x - BossPos.x - _OtherCollider->GetScale().x, PlayerPos.y - BossPos.y, -10.f));
            hitFx->Transform()->SetRelativeScale(Vec3(95.f, 134.f, 1.f));
        }
        hitFx->FlipbookPlayer()->Play(random, 12, false);

    }

}

void CBoss1Hit::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBoss1Hit::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    if (_OtherObject->GetLayerIdx() == 12)  // 플레이어 공격 레이어인 경우
    {
        m_IsHit = false;  // hit 상태 해제
        m_HitTime = 0.f;  // 타이머도 리셋
    }
}
