#include "pch.h"
#include "CHpReduceFx.h"
#include <Engine/CLevelMgr.h>
#include "CPlayerScript.h"
#include <Engine/CDataMgr.h>

CHpReduceFx::CHpReduceFx()
    : CScript(SCRIPT_TYPE::HPREDUCEFX)
    , m_MaxHP(0)
    , m_CurrentHP(0)
    , m_DistHp(0)
    , m_Trigger(false)
{
}

CHpReduceFx::~CHpReduceFx()
{
}


void CHpReduceFx::SaveComponent(FILE* _File)
{
}

void CHpReduceFx::LoadComponent(FILE* _File)
{
}

void CHpReduceFx::Begin()
{

    // 100% 절대적인 위치를 잡아주어야함.
    m_FullHpPos = Vec3(-518.f, 311.f, 30.f);
    m_FullHpScale = Vec3(150.f, 32.f, 0.f);
    CGameObject* pPlayer = CLevelMgr::GetInst()->FindObjectByName(L"Player");
    CPlayerScript* pScript = (CPlayerScript*)pPlayer->GetScripts()[0];
    if (pScript)
    {
        m_MaxHP = pScript->GetInfo().Max_Hp;
        m_CurrentHP = CDataMgr::GetInst()->GetPlayerData().Cur_Hp;
    }
    SettingHp((int)m_CurrentHP);
}

void CHpReduceFx::Tick()
{
    if (m_Trigger)
    {
        // 현재 값 가져오기
        float currentScaleX = Transform()->GetRelativeScale().x;
        float currentPosX = Transform()->GetRelativePos().x;

        // 목표값 계산 (현재 HP 기준)
        float targetHpRatio = m_CurrentHP / m_MaxHP;
        float targetScaleX = m_FullHpScale.x * targetHpRatio;
        float targetPosX = m_FullHpPos.x - (m_FullHpScale.x - targetScaleX) * 0.5f;

        // 보간 속도 설정
        float lerpSpeed = 2.5f * DT;

        // X값에 대해서만 보간 적용
        float newScaleX = currentScaleX + (targetScaleX - currentScaleX) * lerpSpeed;
        float newPosX = currentPosX + (targetPosX - currentPosX) * lerpSpeed;

        // 현재 Transform 값 가져오기 (y, z 유지를 위해)
        Vec3 currentScale = Transform()->GetRelativeScale();
        Vec3 currentPos = Transform()->GetRelativePos();

        // 새로운 Transform 값 적용 (x만 변경)
        Transform()->SetRelativeScale(Vec3(newScaleX, currentScale.y, currentScale.z));
        Transform()->SetRelativePos(Vec3(newPosX, currentPos.y, currentPos.z));

        // 목표 x값에 거의 도달했으면 트리거 해제
        if (abs(newScaleX - targetScaleX) < 0.01f)
        {
            m_Trigger = false;
            Transform()->SetRelativeScale(Vec3(targetScaleX, currentScale.y, currentScale.z));
            Transform()->SetRelativePos(Vec3(targetPosX, currentPos.y, currentPos.z));
        }
    }
}

Vec3 CHpReduceFx::Lerp(const Vec3& start, const Vec3& end, float t)
{
    t = (t < 0.f) ? 0.f : (t > 1.f) ? 1.f : t;
    return Vec3(
        start.x + (end.x - start.x) * t,
        start.y + (end.y - start.y) * t,
        start.z + (end.z - start.z) * t
    );
}

void CHpReduceFx::SettingHp(int _Hp)
{
    m_Trigger = true;
    // Update the target HP value
    m_CurrentHP = (float)_Hp;

    // Clamp HP values
    if (m_CurrentHP > m_MaxHP)
        m_CurrentHP = m_MaxHP;
    if (m_CurrentHP < 0.f)
        m_CurrentHP = 0.f;

    m_DistHp = m_MaxHP - m_CurrentHP;
}