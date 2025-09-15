#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"


GAMEOBJECT_SAVE CPrefab::g_ObjectSave = nullptr;
GAMEOBJECT_LOAD CPrefab::g_ObjectLoad = nullptr;


CPrefab::CPrefab()
	: CAsset(ASSET_TYPE::PREFAB, false)
	, m_ProtoObj(nullptr)
{
}

CPrefab::CPrefab(const CPrefab& _Origin)
	: CAsset(_Origin)
	, m_ProtoObj(nullptr)
{
	if (nullptr != _Origin.m_ProtoObj)
	{
		m_ProtoObj = _Origin.m_ProtoObj->Clone();
	}
}

CPrefab::~CPrefab()
{
	if (nullptr != m_ProtoObj)
		delete m_ProtoObj;
}

void CPrefab::SetProtoObject(CGameObject* _Object)
{
	if (m_ProtoObj)
	{
		delete m_ProtoObj;
	}

	m_ProtoObj = _Object;
}

CGameObject* CPrefab::Instantiate()
{
	CGameObject* pNewObj = m_ProtoObj->Clone();
	LoadLayerIndices(pNewObj);
	return pNewObj;
}

int CPrefab::Save(const wstring& _FilePath)
{
	m_LayerIndices.clear();
	SaveLayerIndices(m_ProtoObj);

	FILE* File = nullptr;
	_wfopen_s(&File, _FilePath.c_str(), L"wb");

	// 레이어 인덱스 맵 저장
	size_t mapSize = m_LayerIndices.size();
	fwrite(&mapSize, sizeof(size_t), 1, File);

	for (const auto& pair : m_LayerIndices)
	{
		SaveWString(pair.first, File);
		fwrite(&pair.second, sizeof(int), 1, File);
	}


	wstring key = GetKey();
	wstring path = GetRelativePath();

	SaveWString(key, File);
	SaveWString(path, File);


	bool Exist = m_ProtoObj;
	fwrite(&Exist, sizeof(bool), 1, File);

	if (m_ProtoObj)
	{
		g_ObjectSave(m_ProtoObj, File);
	}

	fclose(File);

	return S_OK;
}

void CPrefab::SaveLayerIndices(CGameObject* _Object)
{
	// 객체의 이름과 레이어 인덱스를 맵에 저장
	m_LayerIndices[_Object->GetName()] = _Object->GetLayerIdx();

	// 자식 객체들에 대해서도 동일한 작업 수행
	const vector<CGameObject*>& vecChild = _Object->GetChild();
	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		SaveLayerIndices(vecChild[i]);
	}
}

void CPrefab::LoadLayerIndices(CGameObject* _Object)
{
	// 맵에서 해당 객체의 레이어 인덱스를 찾아 설정
	auto iter = m_LayerIndices.find(_Object->GetName());
	if (iter != m_LayerIndices.end())
	{
		_Object->SetLayerIdx(iter->second);
	}

	// 자식 객체들에 대해서도 동일한 작업 수행
	const vector<CGameObject*>& vecChild = _Object->GetChild();
	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		LoadLayerIndices(vecChild[i]);
	}
}

int CPrefab::Load(const wstring& _FilePath)
{
	FILE* File = nullptr;
	_wfopen_s(&File, _FilePath.c_str(), L"rb");

	// 레이어 인덱스 맵 로드
	size_t mapSize = 0;
	fread(&mapSize, sizeof(size_t), 1, File);

	m_LayerIndices.clear();
	for (size_t i = 0; i < mapSize; ++i)
	{
		wstring objName;
		int layerIdx;
		LoadWString(objName, File);
		fread(&layerIdx, sizeof(int), 1, File);
		m_LayerIndices[objName] = layerIdx;
	}

	wstring key = GetKey();
	wstring path = GetRelativePath();

	LoadWString(key, File);
	LoadWString(path, File);

	SetKey(key);
	SetRelativePath(path);

	bool Exist = false;
	fread(&Exist, sizeof(bool), 1, File);

	if (Exist)
	{
		m_ProtoObj = g_ObjectLoad(File);
	}

	fclose(File);

	return S_OK;
}
