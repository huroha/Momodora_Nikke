#include "pch.h"
#include "CHpControll.h"

#include "CPlayerScript.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CDataMgr.h>

CHpControll::CHpControll()
    : CScript((UINT)SCRIPT_TYPE::HPCONTROLL)
    , m_MaxHP(100.f)
    , m_CurrentHP(100.f)

{
}

CHpControll::~CHpControll()
{
}


void CHpControll::SaveComponent(FILE* _File)
{
}

void CHpControll::LoadComponent(FILE* _File)
{
}

void CHpControll::Begin()
{
    // 100% 절대적인 위치를 잡아주어야함.
    m_FullHpPos = Vec3(-518.f,311.f,30.f);
    m_FullHpScale = Vec3(150.f,32.f,0.f);
    
    CGameObject* pPlayer = CLevelMgr::GetInst()->FindObjectByName(L"Player");
    CPlayerScript* pScript = (CPlayerScript*)pPlayer->GetScripts()[0];
    if (pScript)
    {
        m_MaxHP = pScript->GetInfo().Max_Hp;
        m_CurrentHP = CDataMgr::GetInst()->GetPlayerData().Cur_Hp;
        //m_CurrentHP = pScript->GetInfo().Cur_Hp;
    }
    SettingHp((int)m_CurrentHP);
}

void CHpControll::Tick()
{
 }

void CHpControll::SettingHp(int _Hp)
{
    // HP 업데이트
    m_CurrentHP = (float)_Hp;

    // HP가 최대값을 넘지 않도록 제한
    if (m_CurrentHP > m_MaxHP)
        m_CurrentHP = m_MaxHP;
    // HP가 0 미만이 되지 않도록 제한
    if (m_CurrentHP < 0.f)
        m_CurrentHP = 0.f;
    // 현재 HP 비율에 따른 스케일 팩터 계산
    float hpRatio = m_CurrentHP / m_MaxHP;

    // Y와 Z 차원은 유지하면서 새로운 스케일 계산
    Vec3 newScale = Vec3(m_FullHpScale.x * hpRatio, m_FullHpScale.y, m_FullHpScale.z);

    // 바가 왼쪽에 고정되도록 위치 조정 계산
    float posAdjust = (m_FullHpScale.x - newScale.x) * 0.5f;
    Vec3 newPos = Vec3(m_FullHpPos.x - posAdjust, m_FullHpPos.y, m_FullHpPos.z);

    // 화면 밖으로 나가지 않도록 위치 제한
    if (newPos.x < -595.f)
        newPos.x = -595.f;

    // 새로운 트랜스폼 값 적용
    Transform()->SetRelativeScale(newScale);
    Transform()->SetRelativePos(newPos);

}
