#include "pch.h"
#include "CStructuredBuffer.h"

#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
	: m_Desc{}
	, m_ElementCount(0)
	, m_ElementSize(0)
	, m_Type(SB_TYPE::SRV_ONLY)
	, m_SysMemMove(false)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}

int CStructuredBuffer::Create(UINT _ElementSize, UINT _ElementCount
	, SB_TYPE _Type, bool _SysMemMove, void* _SysMem)
{
	assert(!(_ElementSize % 16));

	m_SB_Main = nullptr;
	m_SB_Write = nullptr;
	m_SB_Read = nullptr;

	m_SRV = nullptr;
	m_UAV = nullptr;

	m_ElementSize = _ElementSize;
	m_ElementCount = _ElementCount;
	m_Type = _Type;
	m_SysMemMove = _SysMemMove;

	// Desc ����
	m_Desc.ByteWidth = m_ElementSize * m_ElementCount;
	m_Desc.Usage = D3D11_USAGE_DEFAULT;
	m_Desc.CPUAccessFlags = 0;
	m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	// UAV �߰�
	if (SB_TYPE::SRV_UAV == m_Type)
		m_Desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;

	m_Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	m_Desc.StructureByteStride = m_ElementSize;

	HRESULT hr = E_FAIL;
	if (nullptr != _SysMem)
	{
		D3D11_SUBRESOURCE_DATA Sub = {};
		Sub.pSysMem = _SysMem;
		hr = DEVICE->CreateBuffer(&m_Desc, &Sub, m_SB_Main.GetAddressOf());
	}

	else
	{
		hr = DEVICE->CreateBuffer(&m_Desc, nullptr, m_SB_Main.GetAddressOf());
	}

	if (FAILED(hr))
		return E_FAIL;

	// ShaderResourceView ����
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};

	SRVDesc.BufferEx.NumElements = m_ElementCount;
	SRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;

	if (FAILED(DEVICE->CreateShaderResourceView(m_SB_Main.Get(), &SRVDesc, m_SRV.GetAddressOf())))
	{
		return E_FAIL;
	}

	// UnorderedAccessView ����
	if (SB_TYPE::SRV_UAV == m_Type)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};

		UAVDesc.Buffer.NumElements = m_ElementCount;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

		if (FAILED(DEVICE->CreateUnorderedAccessView(m_SB_Main.Get(), &UAVDesc, m_UAV.GetAddressOf())))
		{
			return E_FAIL;
		}
	}


	if (m_SysMemMove)
	{
		D3D11_BUFFER_DESC WBDesc = m_Desc;
		WBDesc.Usage = D3D11_USAGE_DYNAMIC;
		WBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		WBDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if (FAILED(DEVICE->CreateBuffer(&WBDesc, nullptr, m_SB_Write.GetAddressOf())))
			return E_FAIL;

		D3D11_BUFFER_DESC RBDesc = m_Desc;
		RBDesc.Usage = D3D11_USAGE_DEFAULT;
		RBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		RBDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if (FAILED(DEVICE->CreateBuffer(&RBDesc, nullptr, m_SB_Read.GetAddressOf())))
			return E_FAIL;
	}

	return S_OK;
}

void CStructuredBuffer::SetData(void* _SystemMem, UINT _ElementCount)
{
	// SysMemMove ����� �����ִ��� Ȯ���Ѵ�.
	assert(m_SysMemMove);

	// ���� ���� ���۸� �����Ѵ�.
	D3D11_MAPPED_SUBRESOURCE tMapSub = {};
	CONTEXT->Map(m_SB_Write.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tMapSub);

	// �����͸� �����Ѵ�.
	if (_ElementCount == 0)
		_ElementCount = m_ElementCount;
	memcpy(tMapSub.pData, _SystemMem, m_ElementSize * _ElementCount);

	// �����͸� ���� ���۷� ������.
	CONTEXT->Unmap(m_SB_Write.Get(), 0);

	// ������ۿ� ������ �����͸� Main ���۷� �ű��.
	CONTEXT->CopyResource(m_SB_Main.Get(), m_SB_Write.Get());
}

void CStructuredBuffer::GetData(void* _SysDest)
{
	// SysMemMove ����� �����ִ��� Ȯ���Ѵ�.
	assert(m_SysMemMove);

	// Main ���ۿ� �ִ� ������ Read ���۷� �ű��.
	CONTEXT->CopyResource(m_SB_Read.Get(), m_SB_Main.Get());

	// Read ���۸� �����Ѵ�.
	D3D11_MAPPED_SUBRESOURCE tMapSub = {};
	CONTEXT->Map(m_SB_Read.Get(), 0, D3D11_MAP_READ, 0, &tMapSub);

	// �����͸� �������� �����Ѵ�.	
	memcpy(_SysDest, tMapSub.pData, m_ElementSize * m_ElementCount);

	// ���� ����
	CONTEXT->Unmap(m_SB_Read.Get(), 0);
}


void CStructuredBuffer::Binding(UINT _RegisterNum)
{
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}


void CStructuredBuffer::Clear(UINT _RegisterNum)
{
	ID3D11ShaderResourceView* SRV = nullptr;
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, &SRV);
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, &SRV);
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, &SRV);
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, &SRV);
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, &SRV);
}

void CStructuredBuffer::Binding_CS_SRV(UINT _RegisterNum)
{
	CONTEXT->CSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void CStructuredBuffer::Binding_CS_UAV(UINT _RegisterNum)
{
	assert(m_UAV.Get());
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(_RegisterNum, 1, m_UAV.GetAddressOf(), &i);
}

void CStructuredBuffer::Clear_CS_SRV(UINT _RegisterNum)
{
	ID3D11ShaderResourceView* SRV = nullptr;
	CONTEXT->CSSetShaderResources(_RegisterNum, 1, &SRV);
}

void CStructuredBuffer::Clear_CS_UAV(UINT _RegisterNum)
{
	ID3D11UnorderedAccessView* UAV = nullptr;
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(_RegisterNum, 1, &UAV, &i);
}
