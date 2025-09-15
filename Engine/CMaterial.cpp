#include "pch.h"
#include "CMaterial.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CTexture.h"


CMaterial::CMaterial(bool _EngineRes)
	: CAsset(ASSET_TYPE::MATERIAL, _EngineRes)
	, m_Const{}
{

}
CMaterial::CMaterial(const CMaterial& _Origin)
	: CAsset(_Origin)
	, m_Shader(_Origin.m_Shader)
	, m_Const(_Origin.m_Const)
	, m_arrTex{}
	, m_SharedMtrl(_Origin.m_SharedMtrl)
{
	for (UINT i = 0; i < (UINT)TEX_PARAM::TEX_END; ++i)
	{
		m_arrTex[i] = _Origin.m_arrTex[i];
	}
}


CMaterial::~CMaterial()
{
}



void CMaterial::Binding()
{
	if (nullptr == m_Shader)
		return;


	// Texture ���ε�
	for (UINT i = 0; i < TEX_END; ++i)
	{
		if (nullptr == m_arrTex[i])
		{
			// ��� ���ۿ� �ؽ��� �����Ͱ� ��������ʴٸ� Ŭ���� ����.
			m_Const.bTex[i] = 0;
			CTexture::Clear(i);
			continue;
		}
		m_Const.bTex[i] = 1;
		m_arrTex[i]->Binding(i);
	}

	// ��� ������ ���ε�
	static CConstBuffer* pMtrlCB = CDevice::GetInst()->GetCB(CB_TYPE::MATERIAL);
	pMtrlCB->SetData(&m_Const);
	pMtrlCB->Binding();

	// Shader Binding
	m_Shader->Binding();
}

void* CMaterial::GetScalarParam(SCALAR_PARAM _Type)
{
	switch (_Type)
	{
	case INT_0:
	case INT_1:
	case INT_2:
	case INT_3:
		return m_Const.iArr + _Type;
	case FLOAT_0:
	case FLOAT_1:
	case FLOAT_2:
	case FLOAT_3:
		return m_Const.fArr + (_Type - FLOAT_0);
	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
		return m_Const.v2Arr + (_Type - VEC2_0);
	case VEC4_0:
	case VEC4_1:
	case VEC4_2:
	case VEC4_3:
		return m_Const.v4Arr + (_Type - VEC4_0);
	case MAT_0:
	case MAT_1:
		return m_Const.mat + (_Type - MAT_0);
	}

	assert(nullptr);
}


void CMaterial::SetTexParam(TEX_PARAM _Param, Ptr<CTexture> _tex)
{
	m_arrTex[_Param] = _tex;
}


CMaterial* CMaterial::Clone()
{
	CMaterial* pCloneMtrl = new CMaterial(*this);
	pCloneMtrl->m_SharedMtrl = this;
	return pCloneMtrl;
}

int CMaterial::Save(const wstring& _FilePath)
{
	// m_SharedMtrl ���� ������ ����Ų�� == ���������̴�.
	// ���� ������ �����Ϸ��� �ߴ�. ==> ����
	// ���� ���� = ������ �÷��̵Ǵ� ���߿� �Ͻ������� ���� ���� ������ ����
	assert(!m_SharedMtrl.Get());

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");
	assert(pFile);
	SaveWString(GetName(), pFile);
	SaveAssetRef(m_Shader, pFile);

	fwrite(&m_Const, sizeof(MtrlConst), 1, pFile);

	for (int i = 0; i < (int)TEX_PARAM::TEX_END; ++i)
	{
		SaveAssetRef(m_arrTex[i], pFile);
	}

	fclose(pFile);

	return S_OK;
}


int CMaterial::Load(const wstring& _strFilePath)
{
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");
	assert(pFile);
	wstring name;
	LoadWString(name, pFile);
	SetName(name);
	LoadAssetRef(m_Shader, pFile);

	fread(&m_Const, sizeof(MtrlConst), 1, pFile);

	for (int i = 0; i < (int)TEX_PARAM::TEX_END; ++i)
	{
		LoadAssetRef(m_arrTex[i], pFile);
	}

	fclose(pFile);

	return S_OK;
}