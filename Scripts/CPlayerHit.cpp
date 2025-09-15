#include "pch.h"
#include "CPlayerHit.h"


#include "CPlayerScript.h"
#include "CHpControll.h"
#include "CMpControll.h"
#include "CHpReduceFx.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CSoundMgr.h>

#define BUSTOBJ m_vecObj[0]
#define HPOBJ m_vecObj[1]
#define MPOBJ m_vecObj[2]
#define BUSTTIMEOBJ m_vecObj[3]
#define HPFXOBJ m_vecObj[4]



CPlayerHit::CPlayerHit()
    : CScript((UINT)SCRIPT_TYPE::PLAYERHIT)
    , m_Prefab(nullptr)
    , m_IsHit(false)
    , m_HitTime(0.f)
    , m_Bust(false)
    , m_BustTime(0.f)
    , m_HitDirRight(false)
    , m_HitCool(false)
    , m_CoolTime(0.f)

{
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::PREFAB, "PostProcess", &m_Prefab });
}

CPlayerHit::~CPlayerHit()
{

}

void CPlayerHit::SaveComponent(FILE* _File)
{
}

void CPlayerHit::LoadComponent(FILE* _File)
{
}

void CPlayerHit::Begin()
{
    m_Prefab = CAssetMgr::GetInst()->Load<CPrefab>(L"Prefab\\Bust_PostProcess.pref", L"Prefab\\Bust_PostProcess.pref");

    CGameObject* pObj;
    pObj = nullptr;
    m_vecObj.push_back(pObj);
    pObj = CLevelMgr::GetInst()->FindObjectByName(L"HP_Remain");
    m_vecObj.push_back(pObj);
    pObj = CLevelMgr::GetInst()->FindObjectByName(L"MP_Remain");
    m_vecObj.push_back(pObj);
    pObj = CLevelMgr::GetInst()->FindObjectByName(L"BustTime_Effect");
    m_vecObj.push_back(pObj);
    pObj = CLevelMgr::GetInst()->FindObjectByName(L"playerHp_Effect");
    m_vecObj.push_back(pObj);

    BUSTTIMEOBJ->GetRenderComponent()->SetActive(false);

}

void CPlayerHit::Tick()
{
    if (m_IsHit)
    {
        m_HitTime += DT;

        // 1.2초가 지났고, 아직도 overlap 상태라면
        if (m_HitTime >= 1.2f)
        {
            GetOwner()->GetParent()->FlipbookPlayer()->StartHitEffect(0.6f, Vec3(1.f, 1.f, 1.f));
            UI_CalculateHPMP(-20, true);
            GetOwner()->GetParent()->StateMachine()->ChangeState(L"CPlayerHitState");
            m_HitTime = 0.f; // 타이머 리셋
        }

    }
    else
        m_HitTime = 0.f; // hit 상태가 아니면 타이머도 리셋




    if (m_HitCool)
    {
        m_CoolTime += DT;
    }
    if (m_CoolTime > 0.8f)
    {
        m_HitCool = false;
        m_CoolTime = 0;
    }

    if (m_Bust)
    {
        m_BustTime += DT;
    }

    if (m_BustTime > 10.f)
    {
        m_BustTime = 0.f;
        m_Bust = false;
        DestroyObject(BUSTOBJ);
        BUSTOBJ = nullptr;
        ((CPlayerScript*)GetOwner()->GetParent()->GetScripts()[0])->GetInfo().IsBust = false;
        BUSTTIMEOBJ->GetRenderComponent()->SetActive(false);
    }


    if (KEY_TAP(KEY::R) && ((CPlayerScript*)GetOwner()->GetParent()->GetScripts()[0])->GetInfo().Cur_Mp == 100)
    {
        if (!m_Bust)
        {
            // MP를 한번에 감소
            UI_CalculateHPMP(-100, false);
            ((CPlayerScript*)GetOwner()->GetParent()->GetScripts()[0])->GetInfo().IsBust = true;
            m_Bust = true;

            Vec3 Position = GetOwner()->GetParent()->Transform()->GetRelativePos();
            BUSTOBJ = Instantiate_Edit(m_Prefab, Vec3(Position.x, Position.y - 10.f, 0.f), 2);
            BUSTOBJ->Transform()->SetSpawnTime(g_Data.Time);
            BUSTTIMEOBJ->GetRenderComponent()->SetActive(true);
            CSoundMgr::GetInst()->PlayFX(L"Bust", 0.7f);
        }
    }
}

// 몸체 콜라이더에 들어있음!!!!
void CPlayerHit::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

    if ((_OtherObject->GetLayerIdx() == 14 || _OtherObject->GetLayerIdx() == 6) && !m_HitCool)
    {
        GetOwner()->GetParent()->FlipbookPlayer()->StartHitEffect(0.6f, Vec3(1.f, 1.f, 1.f));

        m_IsHit = true;
        m_HitCool = true;

        if (nullptr != HPOBJ)
        {
            UI_CalculateHPMP(-20, true);
        }
        if (_OtherObject->GetParent() == nullptr)
        {
            if (_OtherObject->Transform()->GetRelativePos().x > GetOwner()->GetParent()->Transform()->GetRelativePos().x)
            {
                m_HitDirRight = true;
            }
            else
            {
                m_HitDirRight = false;
            }
        }
        else
        {
            if (_OtherObject->GetParent()->Transform()->GetRelativePos().x > GetOwner()->GetParent()->Transform()->GetRelativePos().x)
            {
                m_HitDirRight = true;
            }
            else
            {
                m_HitDirRight = false;
            }
        }
       


        GetOwner()->GetParent()->StateMachine()->ChangeState(L"CPlayerHitState");
       

    
    }


    
    

}

void CPlayerHit::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    // 몬스터를 지나갈 수없게 강제로 포지션 세팅해주는 코드 폐기
   
}

void CPlayerHit::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    if (_OtherObject->GetLayerIdx() == 14 || _OtherObject->GetLayerIdx() == 6)  // 몬스터 레이어인 경우
    {
        m_IsHit = false;  // hit 상태 해제
        m_HitTime = 0.f;  // 타이머도 리셋
    }
}

void CPlayerHit::UI_CalculateHPMP(int _value, bool _isHP)
{
    if (!m_Bust)
    {
        CPlayerScript* pPlayerScript = ((CPlayerScript*)GetOwner()->GetParent()->GetScripts()[0]);
        if (_isHP)
        {

            pPlayerScript->CalcHp(_value);
            ((CHpControll*)HPOBJ->GetScripts()[0])->SettingHp(pPlayerScript->GetInfo().Cur_Hp);
            ((CHpReduceFx*)HPFXOBJ->GetScripts()[0])->SettingHp(pPlayerScript->GetInfo().Cur_Hp);


        }
        else
        {

            pPlayerScript->CalcMp(_value);
            ((CMpControll*)MPOBJ->GetScripts()[0])->SettingMp(pPlayerScript->GetInfo().Cur_Mp);

        }
    }
   
}
