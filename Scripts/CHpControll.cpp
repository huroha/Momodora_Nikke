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
    // 100% �������� ��ġ�� ����־����.
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
    Vec3 newScale = Vec3(m_FullHpScale.x * hpRatio, m_FullHpScale.y, m_FullHpScale.z);

    // �ٰ� ���ʿ� �����ǵ��� ��ġ ���� ���
    float posAdjust = (m_FullHpScale.x - newScale.x) * 0.5f;
    Vec3 newPos = Vec3(m_FullHpPos.x - posAdjust, m_FullHpPos.y, m_FullHpPos.z);

    // ȭ�� ������ ������ �ʵ��� ��ġ ����
    if (newPos.x < -595.f)
        newPos.x = -595.f;

    // ���ο� Ʈ������ �� ����
    Transform()->SetRelativeScale(newScale);
    Transform()->SetRelativePos(newPos);

}
