#include "pch.h"
#include "CBoss1Script.h"

#include <Engine/CLevelMgr.h>

#define ROARPARTICLE    m_vecObj[0]
#define ROARPOST        m_vecObj[1]
#define BRESSOBJ        m_vecObj[2]
#define DUSTOBJ         m_vecObj[3]
    

CBoss1Script::CBoss1Script()
    : CScript((UINT)SCRIPT_TYPE::BOSS1SCRIPT)
    , m_Target(nullptr)
    , m_PrefTime(0.f)
    , m_RoarAttackOn(false)
    , m_Once(false)
    , m_HP(500)
    , m_AtkCount(0)
    , m_BressOn(false)
    , m_BressOnce(false)
    , m_BressTime(0.f)
    , m_DustOn(false)
    , m_DustTime(0.f)
    , m_DeadOnce(false)
{
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Player HP", &m_HP });
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::PREFAB, "RoarPost", &m_PostPref });
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::PREFAB, "RoarParticle", &m_ParticlePref });
}

CBoss1Script::~CBoss1Script()
{
}

void CBoss1Script::SaveComponent(FILE* _File)
{
}


void CBoss1Script::LoadComponent(FILE* _File)
{
}

void CBoss1Script::Begin()
{
    if (GetOwner()->GetComponent(COMPONENT_TYPE::STATEMACINE) != nullptr)
        StateMachine()->ChangeState(L"CBoss1None");

    m_PostPref = CAssetMgr::GetInst()->Load<CPrefab>(L"Prefab\\Roar_PostProcess.pref", L"Prefab\\Roar_PostProcess.pref");
    m_ParticlePref = CAssetMgr::GetInst()->Load<CPrefab>(L"Prefab\\Roar_Particle.pref", L"Prefab\\Roar_Particle.pref");
    m_DustParticlePref = CAssetMgr::GetInst()->Load<CPrefab>(L"Prefab\\Dust_Particle.pref", L"Prefab\\Dust_Particle.pref");

    CGameObject* pObj;
    pObj = nullptr;
    m_vecObj.push_back(pObj);
    m_vecObj.push_back(pObj);

    pObj = CLevelMgr::GetInst()->FindObjectByName(L"Bress");
    m_vecObj.push_back(pObj);
    m_vecObj.push_back(nullptr);
    BRESSOBJ->GetRenderComponent()->SetActive(false);

    GetOwner()->GetChild()[6]->GetRenderComponent()->SetActive(false);
    GetOwner()->GetChild()[6]->Collider2D()->Deactivate();
    GetOwner()->GetChild()[6]->StateMachine()->ChangeState(L"CQuakeEffectNone");

    RigidBody2D()->SetJumpSpeed(900.f);
    RigidBody2D()->SetMaxSpeed(500.f);


}

void CBoss1Script::Tick()
{



    // 플레이어 위치에 따른 방향 전환
    if (m_Target != nullptr && GetOwner()->GetChild()[6]->StateMachine()->GetCurStateName() != L"CQuakeEffect" && !m_DeadOnce)
    {
        // 플레이어가 오른쪽에 있으면, 오른쪽을 봐야함.
        if (m_Target->GetParent()->Transform()->GetRelativePos().x > GetOwner()->Transform()->GetRelativePos().x)
        {
             if (GetOwner()->Transform()->GetRelativeScale().x < 0)
            {
                if (GetOwner()->StateMachine()->GetCurStateName() != L"CBoss1Attack" &&
                    GetOwner()->StateMachine()->GetCurStateName() != L"CBoss1Attack2")
                {
                    GetOwner()->Transform()->SetRelativeScale(Vec3(512.f, 320.f, 1.f));
                    vector<CGameObject*> Child = GetOwner()->GetChild();
                    Child[0]->Collider2D()->SetOffset(Vec2(46.f, -5.f));
                    Child[1]->Collider2D()->SetOffset(Vec2(440.f, 0.f));
                    Child[2]->Collider2D()->SetOffset(Vec2(180.f, -60.f));
                    Child[4]->Collider2D()->SetOffset(Vec2(10.f, -86.f));
                }

            }
        }
        else
        {
            if (GetOwner()->Transform()->GetRelativeScale().x > 0)
            {
                if (GetOwner()->StateMachine()->GetCurStateName() != L"CBoss1Attack" &&
                    GetOwner()->StateMachine()->GetCurStateName() != L"CBoss1Attack2")
                {
                    GetOwner()->Transform()->SetRelativeScale(Vec3(-512.f, 320.f, 1.f));
                    vector<CGameObject*> Child = GetOwner()->GetChild();
                    Child[0]->Collider2D()->SetOffset(Vec2(-46.f, -5.f));
                    Child[1]->Collider2D()->SetOffset(Vec2(-440.f, 0.f));
                    Child[2]->Collider2D()->SetOffset(Vec2(-180.f, -60.f));
                    Child[4]->Collider2D()->SetOffset(Vec2(-10.f, -86.f));
                }
            }
        }
    }

    // 임시 테스트용
    if (m_RoarAttackOn && !m_Once)
    {
        Vec3 Position = GetOwner()->Transform()->GetRelativePos();
        Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();

        float Particlecorrect = 0.f;
        // 오른쪽 방향을 보고있음
        if (vScale.x > 0)
            Particlecorrect = 150.f;
        else
            Particlecorrect = -150.f;

        ROARPARTICLE = Instantiate_Edit(m_ParticlePref, Vec3(Position.x + Particlecorrect, Position.y - 40.f, 0.f), 2);
        ROARPOST = Instantiate_Edit(m_PostPref, Vec3(Position.x+ Particlecorrect, Position.y - 40.f, 0.f), 2);
        ROARPOST->Transform()->SetSpawnTime(g_Data.Time);
        m_Once = true;
        //FlipbookPlayer()->Play(1, 12.f, true);
    }

    if (m_RoarAttackOn)
        m_PrefTime += DT;
    if (m_PrefTime > 1.9f)
    {
        m_RoarAttackOn = false;
        DestroyObject(ROARPARTICLE);
        DestroyObject(ROARPOST);
        ROARPARTICLE = nullptr;
        ROARPOST = nullptr;
        m_PrefTime = 0.f;
        m_Once = false;
    }



    if (m_BressOn)
        m_BressTime += DT;
    if (m_BressTime > 3.f)
    {
        BRESSOBJ->GetRenderComponent()->SetActive(false);
        BRESSOBJ->RigidBody2D()->SetVelocity(Vec3(0.f, 0.f, 0.f));
        BRESSOBJ->Collider2D()->Deactivate();
        m_BressOn = false;
        m_BressOnce = false;
        m_BressTime = 0.f;
    }


    if (GetOwner()->GetChild()[6]->StateMachine()->GetCurStateName() == L"CQuakeEffect" && !m_DustOn)
    {
        m_DustOn = true;
        DUSTOBJ = Instantiate_Edit(m_DustParticlePref, Vec3(80.f, 160.f, 0.f), 2);
    }
    if (m_DustOn)
        m_DustTime += DT;
    if (m_DustTime > 2.1)
    {
        DestroyObject(DUSTOBJ);
        DUSTOBJ = nullptr;
        m_DustTime = 0.f;
        m_DustOn = false;
    }


    if (RigidBody2D()->IsGround() && m_HP == 0 && !m_DeadOnce)
    {
        StateMachine()->ChangeState(L"CBoss1Dead");
        m_DeadOnce = true;
    }

}