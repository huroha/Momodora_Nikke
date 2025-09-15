#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"

CTexture::CTexture()
	: CAsset(ASSET_TYPE::TEXTURE, false)
    , m_Desc{}
{
}

CTexture::~CTexture()
{
}



int CTexture::Load(const wstring& _FilePath)
{
	// ���� -> SystemMem
	wchar_t szExt[50] = {};
	_wsplitpath_s(_FilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50);
	wstring strExt = szExt;

	HRESULT hr = E_FAIL;

	if (strExt == L".dds" || strExt == L".DDS")
	{
		// DDS
		hr = LoadFromDDSFile(_FilePath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, m_Image);
	}
	else if (strExt == L".tga" || strExt == L".TGA")
	{
		// TGA
		hr = LoadFromTGAFile(_FilePath.c_str(), nullptr, m_Image);
	}
	else
	{
		// Window Image Component(WIC)
		// png, jpg, jpeg, bmp
		hr = LoadFromWICFile(_FilePath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, m_Image);
	}

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"�ؽ��� �ε� ����", L"���ҽ� �ε� ����", MB_OK);
		return E_FAIL;
	}

	// System -> GPU
	// m_Image -> m_Tex2D
	// Texture2D ��ü�� ����
	// Texture2D �� �����Ҷ� ����� ShaderResourceView
	CreateShaderResourceView(DEVICE
		, m_Image.GetImages()
		, m_Image.GetImageCount()
		, m_Image.GetMetadata(), m_SRV.GetAddressOf());

	// ������ ShaderResourveView �� �̿��ؼ� ���� ��ü(Texture2D) �� �˾Ƴ���.
	m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());

	// ������ Texture2D �� Desc ������ �˾Ƴ���.
	m_Tex2D->GetDesc(&m_Desc);


	return S_OK;
}

int CTexture::Create(UINT _Width, UINT _Height, DXGI_FORMAT _PixelFormat, UINT _BindFlag, D3D11_USAGE _Usage)
{
	m_Desc.Width = _Width;
	m_Desc.Height = _Height;

	m_Desc.ArraySize = 1;
	m_Desc.Format = _PixelFormat;

	// �ؽ����� �뵵
	m_Desc.BindFlags = _BindFlag;

	// CPU ���� ���� ���Ŀ� ������ �������� �ɼ�
	m_Desc.Usage = _Usage;

	if (m_Desc.Usage == D3D11_USAGE_DYNAMIC)
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else if (m_Desc.Usage == D3D11_USAGE_STAGING)
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	// �Ӹ� ������ 1 ==> ������ ������ 
	m_Desc.MipLevels = 1;

	m_Desc.MiscFlags = 0;
	m_Desc.SampleDesc.Count = 1;
	m_Desc.SampleDesc.Quality = 0;

	if (FAILED(DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf())))
	{
		return E_FAIL;
	}


	// View ����	
	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		if (FAILED(DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf())))
		{
			return E_FAIL;
		}
	}

	else
	{
		if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			if (FAILED(DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}

		if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			if (FAILED(DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}
		if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			if (FAILED(DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}
	}

	return S_OK;
}

int CTexture::Create(ComPtr<ID3D11Texture2D> _Tex2D)
{
	assert(_Tex2D.Get());

	m_Tex2D = _Tex2D;
	m_Tex2D->GetDesc(&m_Desc);

	// View ����	
	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		if (FAILED(DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf())))
		{
			return E_FAIL;
		}
	}

	else
	{
		if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			if (FAILED(DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}

		if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			if (FAILED(DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}
	}

	return S_OK;
}




void CTexture::Binding(int _RegisterNum)
{
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void CTexture::Clear(int _RegisterNum)
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, &pSRV);
}

void CTexture::Binding_SRV_CS(int _RegisterNum)
{
	m_RecentSRVNum = _RegisterNum;
	CONTEXT->CSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void CTexture::Binding_UAV_CS(int _RegisterNum)
{
	assert(m_UAV.Get());

	m_RecentUAVNum = _RegisterNum;
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(_RegisterNum, 1, m_UAV.GetAddressOf(), &i);
}

void CTexture::Clear_SRV_CS()
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->CSSetShaderResources(m_RecentSRVNum, 1, &pSRV);
}

void CTexture::Clear_UAV_CS()
{
	ID3D11UnorderedAccessView* pUAV = nullptr;
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(m_RecentUAVNum, 1, &pUAV, &i);
}