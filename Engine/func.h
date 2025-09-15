#pragma once
#include "CComponent.h"

void CreateObject(class CGameObject* _NewObj, int _LayerIdx, bool _bChildMove);
void DestroyObject(CGameObject* _TargetObj);
void DeleteComponentTask(CComponent* _Component);

void ChangeLevelState(LEVEL_STATE _NextState);
void ChangeLevel(class CLevel* _Level, LEVEL_STATE _NextState);
void AddChild(CGameObject* _Parent, CGameObject* _Child);

bool IsValid(class CGameObject*& _Object);

void DrawDebugRect(Vec4 _Color, Vec3 _Pos, Vec2 _Scale, Vec3 _Rotation, bool _DepthTest, float _Duration);
void DrawDebugRect(Vec4 _Color, const Matrix& _matWorld, bool _DepthTest, float _Duration);
void DrawDebugCircle(Vec4 _Color, Vec3 _Pos, float _Radius, bool _DepthTest, float _Duration);

void SaveWString(const wstring& _str, FILE* _File);
void LoadWString(wstring& _str, FILE* _File);

// 문자열 변환 함수
string WStringToString(const wstring& wstr);  // wstring을 string으로 변환
wstring StringToWString(const string& str);   // string을 wstring으로 변환


#include "Ptr.h"
#include "CAsset.h"

template<typename T>
void SaveAssetRef(Ptr<T> _Asset, FILE* _File)
{
	// 에셋에 대한 참조 정보를 저장
	bool bAsset = _Asset.Get();
	fwrite(&bAsset, sizeof(bool), 1, _File);

	if (bAsset)
	{
		SaveWString(_Asset->GetName(), _File);
		SaveWString(_Asset->GetKey(), _File);
		SaveWString(_Asset->GetRelativePath(), _File);
	}
}

template<typename T>
void LoadAssetRef(Ptr<T>& _Asset, FILE* _File)
{
	bool bAsset;
	fread(&bAsset, sizeof(bool), 1, _File);

	if (bAsset)
	{
		wstring Name, Key, Path;
		LoadWString(Name, _File);
		LoadWString(Key, _File);
		LoadWString(Path, _File);
		_Asset = CAssetMgr::GetInst()->template Load<T>(Key, Path);
		_Asset->SetName(Name);
	}
}

template<typename T>
T* LoadAssetRef(FILE* _File)
{
	Ptr<T> pAsset = nullptr;

	bool bAsset;
	fread(&bAsset, sizeof(bool), 1, _File);

	if (bAsset)
	{
		wstring Name ,Key, Path;
		LoadWString(Name, _File);
		LoadWString(Key, _File);
		LoadWString(Path, _File);
		pAsset = CAssetMgr::GetInst()->template Load<T>(Key, Path);
		pAsset->SetName(Name);
	}

	return pAsset.Get();
}



template<typename T, int Count>
void DeleteArray(T* (&arr)[Count])
{
	for (UINT i = 0; i < Count; ++i)
	{
		DELETE(arr[i]);
	}
}

template<typename T>
void DeleteVec(vector<T>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		DELETE(_vec[i]);
	}
	_vec.clear();
}

template<typename T1, typename T2>
void DeleteMap(map<T1, T2>& _map)
{
	for (const auto& pair : _map)
	{
		delete pair.second;
	}
	_map.clear();
}


