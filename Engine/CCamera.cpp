#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "components.h"
#include "CRenderMgr.h"

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_Priority(-1)
	, m_Far(10000.f)
	, m_AspectRatio(1.f)
	, m_LayerCheck(0)
	, m_FOV(XM_PI / 2.f)
	, m_Width(0)
	, m_Scale(1.f)
{
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	m_Width = (UINT)vResolution.x;
	m_AspectRatio = vResolution.x / vResolution.y;
}

CCamera::CCamera(const CCamera& _Origin)
	: CComponent(_Origin)
	, m_ProjType(_Origin.m_ProjType)
	, m_Priority(_Origin.m_Priority)
	, m_Far(_Origin.m_Far)
	, m_AspectRatio(_Origin.m_AspectRatio)
	, m_LayerCheck(_Origin.m_LayerCheck)
	, m_FOV(_Origin.m_FOV)
	, m_Width(_Origin.m_Width)
	, m_Scale(_Origin.m_Scale)
{
}
CCamera::~CCamera()
{
}

void CCamera::Begin()
{
	// RenderMgr 에 등록
	CRenderMgr::GetInst()->RegisterCamera(this, m_Priority);
}

void CCamera::FinalTick()
{
	// ViewSpace
	// 1. 카메라가 있는곳이 원점
	// 2. 카메라가 바라보는 방향이 Z 축

	// View 행렬 계산
	Vec3 vWorldPos = Transform()->GetRelativePos();
	Matrix matTrans = XMMatrixTranslation(-vWorldPos.x, -vWorldPos.y, -vWorldPos.z);

	Matrix matRot = XMMatrixIdentity();

	Vec3 vR = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vU = Transform()->GetWorldDir(DIR_TYPE::UP);
	Vec3 vF = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	matRot._11 = vR.x;	matRot._12 = vU.x;	matRot._13 = vF.x;
	matRot._21 = vR.y;	matRot._22 = vU.y;	matRot._23 = vF.y;
	matRot._31 = vR.z;	matRot._32 = vU.z;	matRot._33 = vF.z;

	m_matView = matTrans * matRot;

	// Proj 행렬 계산
	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
		m_matProj = XMMatrixOrthographicLH(m_Width * m_Scale, (m_Width / m_AspectRatio) * m_Scale, 1.f, m_Far);
	else
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
}

void CCamera::SortObject()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (m_LayerCheck & (1 << i))
		{
			CLayer* pLayer = pCurLevel->GetLayer(i);

			const vector<CGameObject*>& vecObjects = pLayer->GetObjects();

			for (size_t j = 0; j < vecObjects.size(); ++j)
			{
				CRenderComponent* pRenderCom = vecObjects[j]->GetRenderComponent();

				// 분류 예외조건 검사
				if (nullptr == pRenderCom
					|| nullptr == pRenderCom->GetMesh()
					|| nullptr == pRenderCom->GetMaterial()
					|| nullptr == pRenderCom->GetMaterial()->GetShader())
					continue;

				SHADER_DOMAIN domain = pRenderCom->GetMaterial()->GetShader()->GetDomain();

				switch (domain)
				{
				case SHADER_DOMAIN::DOMAIN_OPAQUE:
					m_vecOpaque.push_back(vecObjects[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_MASK:
					m_vecMask.push_back(vecObjects[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
					m_vecTransparent.push_back(vecObjects[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_PARTICLE:
					m_vecParticle.push_back(vecObjects[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
					m_vecPostprocess.push_back(vecObjects[j]);
					break;
				default:
					assert(nullptr);
				}
			}
		}
	}
}

void CCamera::Render()
{
	g_Trans.matView = m_matView;
	g_Trans.matProj = m_matProj;

	// 불투명
	for (size_t i = 0; i < m_vecOpaque.size(); ++i)
	{
		m_vecOpaque[i]->Render();
	}

	// Mask
	for (size_t i = 0; i < m_vecMask.size(); ++i)
	{
		m_vecMask[i]->Render();
	}

	// 반투명
	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->Render();
	}

	// 파티클
	for (size_t i = 0; i < m_vecParticle.size(); ++i)
	{
		m_vecParticle[i]->Render();
	}

	// 후처리	
	for (size_t i = 0; i < m_vecPostprocess.size(); ++i)
	{
		CRenderMgr::GetInst()->CopyRenderTarget();
		m_vecPostprocess[i]->Render();
	}

	// clear
	m_vecOpaque.clear();
	m_vecMask.clear();
	m_vecTransparent.clear();
	m_vecParticle.clear();
	m_vecPostprocess.clear();
}


void CCamera::LayerCheck(int _LayerIdx)
{
	if (m_LayerCheck & (1 << _LayerIdx))
	{
		m_LayerCheck &= ~(1 << _LayerIdx);
	}

	else
	{
		m_LayerCheck |= (1 << _LayerIdx);
	}
}

void CCamera::SaveComponent(FILE* _File)
{
	fwrite(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fwrite(&m_Far, sizeof(float), 1, _File);
	fwrite(&m_AspectRatio, sizeof(float), 1, _File);
	fwrite(&m_LayerCheck, sizeof(UINT), 1, _File);
	fwrite(&m_Priority, sizeof(int), 1, _File);
	fwrite(&m_FOV, sizeof(float), 1, _File);
	fwrite(&m_Width, sizeof(float), 1, _File);
	fwrite(&m_Scale, sizeof(float), 1, _File);
}

void CCamera::LoadComponent(FILE* _File)
{
	fread(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fread(&m_Far, sizeof(float), 1, _File);
	fread(&m_AspectRatio, sizeof(float), 1, _File);
	fread(&m_LayerCheck, sizeof(UINT), 1, _File);
	fread(&m_Priority, sizeof(int), 1, _File);
	fread(&m_FOV, sizeof(float), 1, _File);
	fread(&m_Width, sizeof(float), 1, _File);
	fread(&m_Scale, sizeof(float), 1, _File);
}