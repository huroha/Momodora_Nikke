#include "pch.h"
#include "CBoss1HpEffect.h"




CBoss1HpEffect::CBoss1HpEffect()
    : CScript((UINT)SCRIPT_TYPE::BOSS1HPEFFECT)
    , m_MaxHP(400.f)
    , m_CurrentHP(400.f)
    , m_Trigger(false)
    , m_DistHp(0.f)
{
}

CBoss1HpEffect::~CBoss1HpEffect()
{
}

void CBoss1HpEffect::SaveComponent(FILE* _File)
{
}

void CBoss1HpEffect::LoadComponent(FILE* _File)
{
}

void CBoss1HpEffect::Begin()
{
    m_OriginPos = Transform()->GetRelativePos();
    m_OriginScale = Transform()->GetRelativeScale();
    m_CurrentHP = m_MaxHP;  
}

void CBoss1HpEffect::Tick()
{
    if (m_Trigger)
    {
        // ���� �� ��������
        float currentScaleX = Transform()->GetRelativeScale().x;
        float currentPosX = Transform()->GetRelativePos().x;

        // ��ǥ�� ��� (���� HP ����)
        float targetHpRatio = m_CurrentHP / m_MaxHP;
        float targetScaleX = m_OriginScale.x * targetHpRatio;
        float targetPosX = m_OriginPos.x - (m_OriginScale.x - targetScaleX) * 0.5f;

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

Vec3 CBoss1HpEffect::Lerp(const Vec3& start, const Vec3& end, float t)
{
    t = (t < 0.f) ? 0.f : (t > 1.f) ? 1.f : t;
    return Vec3(
        start.x + (end.x - start.x) * t,
        start.y + (end.y - start.y) * t,
        start.z + (end.z - start.z) * t
    );
}

void CBoss1HpEffect::SettingHp(int _Hp)
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
