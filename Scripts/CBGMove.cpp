#include "pch.h"
#include "CBGMove.h"

#include <Engine/CLevelMgr.h>

CBGMove::CBGMove()
	: CScript((UINT)SCRIPT_TYPE::BGMOVE)
	, m_Target(nullptr)
	, m_Idx(0)
{
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Speed Idx", &m_Idx });
}

CBGMove::~CBGMove()
{
}

void CBGMove::SaveComponent(FILE* _File)
{
	fwrite(&m_Idx, sizeof(float), 1, _File);
    fwrite(&m_StartBackgroundPos, sizeof(Vec3), 1, _File);
}

void CBGMove::LoadComponent(FILE* _File)
{
	fread(&m_Idx, sizeof(float), 1, _File);
    fread(&m_StartBackgroundPos, sizeof(Vec3), 1, _File);
}

void CBGMove::Begin()
{

	SetTargetObject(CLevelMgr::GetInst()->FindObjectByName(L"Player"));
    if (m_Target)
    {
        m_StartTargetPos = m_Target->Transform()->GetWorldPos();

    }

}

void CBGMove::Tick()
{

	if (m_Target != nullptr)
	{
        // 타겟의 현재 위치 가져오기
        Vec3 vTargetPos = m_Target->Transform()->GetWorldPos();
        Vec3 vCurPos = Transform()->GetWorldPos();
        Vec3 vLerpPos = {};

        // 타겟과의 거리 차이를 이용해 이동
        float targetDiff = vTargetPos.x - m_StartTargetPos.x;  // 타겟의 시작 위치로부터의 이동량
        Vec3 vDest = Vec3(m_StartBackgroundPos.x - targetDiff * ((m_Idx * 0.1f) + 0.2f), 0.f, 0.f);  // 배경도 그만큼 반대로 이동

        float speed = (m_Idx * 0.1f) * m_Idx + 1.5f;
        vLerpPos.x = SmoothStep(vCurPos.x, vDest.x, speed * DT);

        vLerpPos.y = Transform()->GetRelativePos().y;
        vLerpPos.z = Transform()->GetRelativePos().z;

        vLerpPos.x = std::clamp(vLerpPos.x, -1000.f + m_Idx * 25, -760.f);

        Transform()->SetRelativePos(vLerpPos);
	}
}

