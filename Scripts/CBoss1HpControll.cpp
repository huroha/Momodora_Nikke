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
    // HP ������Ʈ
    m_CurrentHP = (float)_Hp;

    // HP�� �ִ밪�� ���� �ʵ��� ����
    if (m_CurrentHP > m_MaxHP)
        m_CurrentHP = m_MaxHP;
    // HP�� 0 �̸��� ���� �ʵ��� ����
    if (m_CurrentHP < 0.f)
        m_CurrentHP = 0.f;
    // ���� HP ������ ���� ������ ���� ���
    float hpRatio = m_CurrentHP / m_MaxHP;

    // Y�� Z ������ �����ϸ鼭 ���ο� ������ ���
    Vec3 newScale = Vec3(m_OriginScale.x * hpRatio, m_OriginScale.y, m_OriginScale.z);

    // �ٰ� ���ʿ� �����ǵ��� ��ġ ���� ���
    float posAdjust = (m_OriginScale.x - newScale.x) * 0.5f;
    Vec3 newPos = Vec3(m_OriginPos.x - posAdjust, m_OriginPos.y, m_OriginPos.z);



    // ���ο� Ʈ������ �� ����
    Transform()->SetRelativeScale(newScale);
    Transform()->SetRelativePos(newPos);
}
