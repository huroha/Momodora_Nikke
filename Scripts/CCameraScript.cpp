#include "pch.h"
#include "CCameraScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CCamera.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CRenderMgr.h>

CCameraScript::CCameraScript()
	: CScript((UINT)SCRIPT_TYPE::CAMERASCRIPT)
    , m_CameraSpeed(100.f)
	, m_Target(nullptr)
	, m_Frequency(0.f)
	, m_Amplitude(0.f)
	, m_Duration(0.f)
	, m_AccTime(0.f)
	, m_CamShake(false)
	, m_DirX(1.f)  
	, m_AmplitudeX(0.f)
	, m_FrequencyX(0.f)
{

}

CCameraScript::~CCameraScript()
{
}

void CCameraScript::Tick()
{
	if (m_Target != nullptr)
	{
		Vec3 vTargetPos = m_Target->Transform()->GetWorldPos();
		float cDir = (m_Target->Transform()->GetRelativeScale().x > 0) ? 150.f : -150.f;
		Vec3 m_vOffsetPos = Vec3(cDir, 0.f, 0.f);
		Vec3 vDest = vTargetPos + m_vOffsetPos;
		Vec3 vCurPos = Transform()->GetRelativePos();

		// 목표 위치와 현재 위치 사이를 보간
		Vec3 vLerpPos;
		float shakeFactor = m_CamShake ? 1.f : 0.f;

		if (m_CamName == L"MainCamera2")
			vLerpPos.x = 0.f + (m_ShakeOffset.x * shakeFactor);
		else
			vLerpPos.x = Lerp(vCurPos.x, vDest.x, 1.f * DT) + (m_ShakeOffset.x * shakeFactor);
		vLerpPos.y = m_OriginY + (m_ShakeOffset.y * shakeFactor);
		vLerpPos.z = 0.f;

		vLerpPos.x = std::clamp(vLerpPos.x, -1400.f, 50.f);
		Transform()->SetRelativePos(vLerpPos);
	}
	else if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
		CameraOrthgraphicMove();
	else
		CameraPerspectiveMove();

	CameraEffect();

}

void CCameraScript::Begin()
{
	SetTargetObject(CLevelMgr::GetInst()->FindObjectByName(L"Player"));
	m_OriginY = Transform()->GetRelativePos().y;
	m_CamName = GetOwner()->GetName();
}



void CCameraScript::CameraEffect()
{
	if (false == m_CamShake)
		return;

	// Y축 흔들림
	m_ShakeOffset.y += m_Amplitude * 4.f * m_Frequency * DT * m_Dir;
	if (m_Amplitude < fabs(m_ShakeOffset.y))
	{
		m_ShakeOffset.y += (fabs(m_ShakeOffset.y) - m_Amplitude) * -m_Dir;
		m_Dir *= -1.f;
	}

	// X축 흔들림 추가
	m_ShakeOffset.x += m_AmplitudeX * 4.f * m_FrequencyX * DT * m_DirX;
	if (m_AmplitudeX < fabs(m_ShakeOffset.x))
	{
		m_ShakeOffset.x += (fabs(m_ShakeOffset.x) - m_AmplitudeX) * -m_DirX;
		m_DirX *= -1.f;
	}

	// 효과 유지시간 체크
	m_AccTime += DT;
	if (m_Duration < m_AccTime)
	{
		// 효과가 만료되면 세팅 값 정리
		m_AccTime = 0.f;
		m_CamShake = false;
		m_ShakeOffset = Vec2(0.f, 0.f);
	}
}

void CCameraScript::CameraOrthgraphicMove()
{
	//Vec3 vWorldPos = Transform()->GetRelativePos();

	//if (KEY_PRESSED(KEY::W))
	//{
	//	vWorldPos.y += DT * m_CameraSpeed;
	//}

	//if (KEY_PRESSED(KEY::S))
	//{
	//	vWorldPos.y -= DT * m_CameraSpeed;
	//}

	//if (KEY_PRESSED(KEY::A))
	//{
	//	vWorldPos.x -= DT * m_CameraSpeed;
	//}

	//if (KEY_PRESSED(KEY::D))
	//{
	//	vWorldPos.x += DT * m_CameraSpeed;
	//}

	//Transform()->SetRelativePos(vWorldPos);


	//float Scale = Camera()->GetScale();

	//if (KEY_PRESSED(KEY::NUM_0))
	//{
	//	Scale += DT;
	//}

	//if (KEY_PRESSED(KEY::NUM_1))
	//{
	//	Scale -= DT;
	//	if (Scale < 0.1f)
	//		Scale = 0.1f;
	//}

	//Camera()->SetScale(Scale);

}

void CCameraScript::CameraPerspectiveMove()
{
	float Speed = m_CameraSpeed;
	if (KEY_PRESSED(KEY::LSHIFT))
		Speed *= 5.f;

	Vec3 vFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Vec3 vRight= Transform()->GetLocalDir(DIR_TYPE::RIGHT);	

	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W))
		vPos += vFront * DT * Speed;
	if(KEY_PRESSED(KEY::S))
		vPos -= vFront * DT * Speed;
	if (KEY_PRESSED(KEY::A))
		vPos -= vRight * DT * Speed;
	if (KEY_PRESSED(KEY::D))
		vPos += vRight * DT * Speed;

	Transform()->SetRelativePos(vPos);

	// 마우스 방향에 따른 오브젝트 회전
	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec3 vRot = Transform()->GetRelativeRotation();

		Vec2 vDir = CKeyMgr::GetInst()->GetMouseDir();
		vRot.y += vDir.x * DT * 15.f;
		vRot.x += vDir.y * DT * 10.f;

		Transform()->SetRelativeRotation(vRot);
	}
}


void CCameraScript::SaveComponent(FILE* _File)
{
	fwrite(&m_CameraSpeed, sizeof(float), 1, _File);
}

void CCameraScript::LoadComponent(FILE* _File)
{
	fread(&m_CameraSpeed, sizeof(float), 1, _File);
}