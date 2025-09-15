#include "pch.h"
#include "CAssetMgr.h"

CAssetMgr::CAssetMgr()
	: m_bAssetChanged(false)
{

}

CAssetMgr::~CAssetMgr()
{

}



Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _Key, UINT _Width, UINT _Height, DXGI_FORMAT _PixelFormat, UINT _BindFlag, D3D11_USAGE _Usage)
{
	Ptr<CTexture> pTex = FindAsset<CTexture>(_Key);

	if (nullptr != pTex)
		return pTex;

	pTex = new CTexture;
	if (FAILED(pTex->Create(_Width, _Height, _PixelFormat, _BindFlag, _Usage)))
	{
		assert(nullptr);
		return nullptr;
	}

	pTex->SetKey(_Key);

	// UI �󿡼� ���� �̸�
	pTex->SetName(_Key);
	m_mapAsset[(UINT)ASSET_TYPE::TEXTURE].insert(make_pair(_Key, pTex.Get()));

	m_bAssetChanged = true;

	return pTex;
}

Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _Key, ComPtr<ID3D11Texture2D> _Tex2D)
{
	Ptr<CTexture> pTex = FindAsset<CTexture>(_Key);

	if (nullptr != pTex)
		return pTex;

	pTex = new CTexture;
	if (FAILED(pTex->Create(_Tex2D)))
	{
		assert(nullptr);
		return nullptr;
	}

	pTex->SetKey(_Key);
	pTex->SetName(_Key);
	m_mapAsset[(UINT)ASSET_TYPE::TEXTURE].insert(make_pair(_Key, pTex.Get()));
	m_bAssetChanged = true;


	return pTex;
}

void CAssetMgr::GetAssetNames(ASSET_TYPE _Type, vector<wstring>& _vecAssetNames)
{
	for (const auto& pair : m_mapAsset[(UINT)_Type])
	{
		_vecAssetNames.push_back(pair.first);
	}
}

void CAssetMgr::GetAssetEntityNames(ASSET_TYPE _Type, vector<wstring>& _vecAssetNames)
{
	for (const auto& pair : m_mapAsset[(UINT)_Type])
	{
		_vecAssetNames.push_back(pair.second->GetName());
	}

}

void CAssetMgr::DeleteAsset(ASSET_TYPE _Type, const wstring& _Key)
{
	map<wstring, Ptr<CAsset>>& mapAsset = m_mapAsset[(UINT)_Type];

	map<wstring, Ptr<CAsset>>::iterator iter = mapAsset.find(_Key);

	if (mapAsset.end() == iter)
		return;

	mapAsset.erase(iter);

	m_bAssetChanged = true;
}