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
    // HP ������Ʈ
    m_CurrentMP = (float)_Mp;

    // HP�� �ִ밪�� ���� �ʵ��� ����
    if (m_CurrentMP > m_MaxMP)
        m_CurrentMP = m_MaxMP;
    // HP�� 0 �̸��� ���� �ʵ��� ����
    if (m_CurrentMP < 0.f)
        m_CurrentMP = 0.f;
    // ���� HP ������ ���� ������ ���� ���
    float mpRatio = m_CurrentMP / m_MaxMP;

    // Y�� Z ������ �����ϸ鼭 ���ο� ������ ���
    Vec3 newScale = Vec3(m_OriginScale.x * mpRatio, m_OriginScale.y, m_OriginScale.z);

    // �ٰ� ���ʿ� �����ǵ��� ��ġ ���� ���
    float posAdjust = (m_OriginScale.x - newScale.x) * 0.5f;
    Vec3 newPos = Vec3(m_OriginPos.x - posAdjust, m_OriginPos.y, m_OriginPos.z);

    // ȭ�� ������ ������ �ʵ��� ��ġ ����
    if (newPos.x < -595.f)
        newPos.x = -595.f;

    // ���ο� Ʈ������ �� ����
    Transform()->SetRelativeScale(newScale);
    Transform()->SetRelativePos(newPos);
}

