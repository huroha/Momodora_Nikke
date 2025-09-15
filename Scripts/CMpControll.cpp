#include "pch.h"
#include "CMpControll.h"

#include <Engine/CDataMgr.h>

CMpControll::CMpControll()
    : CScript((UINT)SCRIPT_TYPE::MPCONTROLL)
    , m_MaxMP(100.f)
    , m_CurrentMP(0.f)
{
}

CMpControll::~CMpControll()
{
}


void CMpControll::SaveComponent(FILE* _File)
{
}

void CMpControll::LoadComponent(FILE* _File)
{
}

void CMpControll::Begin()
{

    m_OriginPos = Transform()->GetRelativePos();//Vec3(-518.f,273.f,30.f);
    m_OriginScale = Transform()->GetRelativeScale();//Vec3(150.f,32.f,0.f);
    m_CurrentMP = CDataMgr::GetInst()->GetPlayerData().Cur_Mp;
    SettingMp((int)m_CurrentMP);

}

void CMpControll::Tick()
{
}

void CMpControll::SettingMp(int _Mp)
{
    // HP 업데이트
    m_CurrentMP = (float)_Mp;

    // HP가 최대값을 넘지 않도록 제한
    if (m_CurrentMP > m_MaxMP)
        m_CurrentMP = m_MaxMP;
    // HP가 0 미만이 되지 않도록 제한
    if (m_CurrentMP < 0.f)
        m_CurrentMP = 0.f;
    // 현재 HP 비율에 따른 스케일 팩터 계산
    float mpRatio = m_CurrentMP / m_MaxMP;

    // Y와 Z 차원은 유지하면서 새로운 스케일 계산
    Vec3 newScale = Vec3(m_OriginScale.x * mpRatio, m_OriginScale.y, m_OriginScale.z);

    // 바가 왼쪽에 고정되도록 위치 조정 계산
    float posAdjust = (m_OriginScale.x - newScale.x) * 0.5f;
    Vec3 newPos = Vec3(m_OriginPos.x - posAdjust, m_OriginPos.y, m_OriginPos.z);

    // 화면 밖으로 나가지 않도록 위치 제한
    if (newPos.x < -595.f)
        newPos.x = -595.f;

    // 새로운 트랜스폼 값 적용
    Transform()->SetRelativeScale(newScale);
    Transform()->SetRelativePos(newPos);
}

