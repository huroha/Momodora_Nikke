#include "pch.h"
#include "CMissileScript.h"


CMissileScript::CMissileScript()
	: CScript((UINT)SCRIPT_TYPE::MISSILESCRIPT)
	, m_Velocity(Vec3(0.f, 500.f, 0.f))
{
}

CMissileScript::~CMissileScript()
{
}

void CMissileScript::Begin()
{
	// CAssetMgr �� ������ �����Ų ������ ����
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl");

	// ���� ���� �Լ�ȭ �ϱ� ���� �ӽ÷� �ߴ� ��.
	//pMtrl = new CMaterial(*pMtrl.Get());
	//MeshRender()->SetMaterial(pMtrl);

	MeshRender()->SetMaterial(pMtrl);
	MeshRender()->GetDynamicMaterial();
	pMtrl->SetScalarParam(INT_0, 1);

	Ptr<CTexture> pMissileTex = CAssetMgr::GetInst()->Load<CTexture>(L"Missile", L"Texture\\missile.png");
	MeshRender()->GetMaterial()->SetTexParam(TEX_0, pMissileTex);
}

void CMissileScript::Tick()
{
	Vec3 vWorldPos = Transform()->GetRelativePos();

	vWorldPos += m_Velocity * DT;

	Transform()->SetRelativePos(vWorldPos);
}

void CMissileScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	DestroyObject(GetOwner());
}

void CMissileScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CMissileScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}


void CMissileScript::SaveComponent(FILE* _File)
{
	fwrite(&m_Velocity, sizeof(float), 1, _File);
}

void CMissileScript::LoadComponent(FILE* _File)
{
	fread(&m_Velocity, sizeof(float), 1, _File);
}