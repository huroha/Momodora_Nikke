#include "pch.h"
#include "CBoss1HpControll.h"



CBoss1HpControll::CBoss1HpControll()
    : CScript((UINT)SCRIPT_TYPE::BOSS1HPCONTROLL)
    , m_MaxHP(400.f)
    , m_CurrentHP(400.f)
{
}

CBoss1HpControll::~CBoss1HpControll()
{
}


void CBoss1HpControll::SaveComponent(FILE* _File)
{
}

void CBoss1HpControll::LoadComponent(FILE* _File)
{
}

void CBoss1HpControll::Begin()
{
    m_OriginPos = Transform()->GetRelativePos();//Vec3(-518.f,273.f,30.f);
    m_OriginScale = Transform()->GetRelativeScale();//Vec3(150.f,32.f,0.f);
}

void CBoss1HpControll::Tick()
{
}

void CBoss1HpControll::SettingHp(int _Hp)
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
    Vec3 newScale = Vec3(m_OriginScale.x * hpRatio, m_OriginScale.y, m_OriginScale.z);

    // 바가 왼쪽에 고정되도록 위치 조정 계산
    float posAdjust = (m_OriginScale.x - newScale.x) * 0.5f;
    Vec3 newPos = Vec3(m_OriginPos.x - posAdjust, m_OriginPos.y, m_OriginPos.z);



    // 새로운 트랜스폼 값 적용
    Transform()->SetRelativeScale(newScale);
    Transform()->SetRelativePos(newPos);
}
