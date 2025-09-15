#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CTimeMgr.h"
CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_IndependentScale(false)
	, m_SpawnTime(g_Data.Time)
{
	m_matWorld = XMMatrixIdentity();
}

CTransform::~CTransform()
{
}

void CTransform::FinalTick()
{
	m_matWorld = XMMatrixIdentity();


	Matrix matScale = XMMatrixScaling(m_RelativeScale.x, m_RelativeScale.y, m_RelativeScale.z);

	Matrix matRotation = XMMatrixRotationX(m_RelativeRotation.x)
					   * XMMatrixRotationY(m_RelativeRotation.y)
					   * XMMatrixRotationZ(m_RelativeRotation.z);

	Matrix matTrans = XMMatrixTranslation(m_RelativePos.x, m_RelativePos.y, m_RelativePos.z);

	// 크기 * 회전 * 이동
	m_matWorld = matScale * matRotation * matTrans;


	// 방향 벡터는 동차좌표에 1이 들어가면안된다!. 변환행렬 적용에 동차좌표를 0으로 해야함.

	// 방향벡터 갱신
	m_LocalDir[(UINT)DIR_TYPE::RIGHT] = Vec3(1.f, 0.f, 0.f);
	m_LocalDir[(UINT)DIR_TYPE::UP] = Vec3(0.f, 1.f, 0.f);
	m_LocalDir[(UINT)DIR_TYPE::FRONT] = Vec3(0.f, 0.f, 1.f);

	for (int i = 0; i < 3; ++i)
	{
		//XMVector3TransformCoord(m_LocalDir[i], matRotation);
		// WorldDir = Local 이므로 셋팅은 동일하게
		m_WorldDir[i] = m_LocalDir[i] = XMVector3TransformNormal(m_LocalDir[i], matRotation);
	}

	// 부모가 있다면
	if (GetOwner()->GetParent())
	{
		const Matrix& matParentWorld = GetOwner()->GetParent()->Transform()->GetWorldMat();
		//m_matWorld *= matParentWorld;

		// 자식 오브젝트는 부모 오브젝트의 크기에 영향을 받는다.
		if (false == m_IndependentScale)
		{
			m_matWorld *= matParentWorld;
		}

		// 자식 오브젝트가 독립적인 크기를 유지하고 싶은 경우
		else
		{
			//부모 객체의 스케일을 먼저 제거한 후 전체 변환을 적용함으로써, 부모의 스케일이 자식에게 두 번 적용되는 문제를 방지
			Vec3 vParentWorldScale = GetOwner()->GetParent()->Transform()->GetWorldScale();

			Matrix matParentScaleInv = XMMatrixScaling(vParentWorldScale.x, vParentWorldScale.y, vParentWorldScale.z);
			matParentScaleInv = XMMatrixInverse(nullptr, matParentScaleInv);

			// 자식 LocalMat * 부모크기-1 * 부모월드행렬(부모크기 * 부모회전 * 부모이동)
			m_matWorld = m_matWorld * matParentScaleInv * matParentWorld;
		}



		// WorldDir 구하기
		m_WorldDir[(UINT)DIR_TYPE::RIGHT] = Vec3(1.f, 0.f, 0.f);
		m_WorldDir[(UINT)DIR_TYPE::UP] = Vec3(0.f, 1.f, 0.f);
		m_WorldDir[(UINT)DIR_TYPE::FRONT] = Vec3(0.f, 0.f, 1.f);

		for (int i = 0; i < 3; ++i)
		{
			m_WorldDir[i] = XMVector3TransformNormal(m_WorldDir[i], m_matWorld);
			m_WorldDir[i].Normalize();
		}
	}

}



Vec3 CTransform::GetWorldScale()
{
	Vec3 vScale = m_RelativeScale;

	if (false == m_IndependentScale)
	{
		CGameObject* pParent = GetOwner()->GetParent();

		while (pParent)
		{
			vScale *= pParent->Transform()->GetRelativeScale();

			if (false == pParent->Transform()->m_IndependentScale)
				pParent = pParent->GetParent();
			else
				break;
		}
	}

	return vScale;
}


void CTransform::Binding()
{
	CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);


	g_Trans.matWorld = m_matWorld;
	g_Trans.matWV = g_Trans.matWorld * g_Trans.matView;
	g_Trans.matWVP = g_Trans.matWV * g_Trans.matProj;
	g_Trans.objectSpawnTime = m_SpawnTime;

	pCB->SetData(&g_Trans);
	pCB->Binding();
}


void CTransform::SaveComponent(FILE* _File)
{
	fwrite(&m_RelativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_RelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_RelativeRotation, sizeof(Vec3), 1, _File);
	fwrite(&m_IndependentScale, sizeof(bool), 1, _File);
}

void CTransform::LoadComponent(FILE* _FILE)
{
	fread(&m_RelativePos, sizeof(Vec3), 1, _FILE);
	fread(&m_RelativeScale, sizeof(Vec3), 1, _FILE);
	fread(&m_RelativeRotation, sizeof(Vec3), 1, _FILE);
	fread(&m_IndependentScale, sizeof(bool), 1, _FILE);
}
