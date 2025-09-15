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

    // 100% �������� ��ġ�� ����־����.
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
        // ���� �� ��������
        float currentScaleX = Transform()->GetRelativeScale().x;
        float currentPosX = Transform()->GetRelativePos().x;

        // ��ǥ�� ��� (���� HP ����)
        float targetHpRatio = m_CurrentHP / m_MaxHP;
        float targetScaleX = m_FullHpScale.x * targetHpRatio;
        float targetPosX = m_FullHpPos.x - (m_FullHpScale.x - targetScaleX) * 0.5f;

        // ���� �ӵ� ����
        float lerpSpeed = 2.5f * DT;

        // X���� ���ؼ��� ���� ����
        float newScaleX = currentScaleX + (targetScaleX - currentScaleX) * lerpSpeed;
        float newPosX = currentPosX + (targetPosX - currentPosX) * lerpSpeed;

        // ���� Transform �� �������� (y, z ������ ����)
        Vec3 currentScale = Transform()->GetRelativeScale();
        Vec3 currentPos = Transform()->GetRelativePos();

        // ���ο� Transform �� ���� (x�� ����)
        Transform()->SetRelativeScale(Vec3(newScaleX, currentScale.y, currentScale.z));
        Transform()->SetRelativePos(Vec3(newPosX, currentPos.y, currentPos.z));

        // ��ǥ x���� ���� ���������� Ʈ���� ����
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