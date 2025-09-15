#include "pch.h"
#include "CConstBuffer.h"
#include "CDevice.h"

CConstBuffer::CConstBuffer(CB_TYPE _Type)
	: m_Desc{}
	, m_Type(_Type)
{
}

CConstBuffer::~CConstBuffer()
{
}


int CConstBuffer::Create(UINT _BufferSize)
{
	assert(!(_BufferSize % 16));

	// 상수버퍼 구조체에 정보 넣어줌
	m_Desc.ByteWidth = _BufferSize;
	m_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_Desc.Usage = D3D11_USAGE_DYNAMIC;

	// Const 용도의 ID3D11Buffer 객체 생성
	if (FAILED(DEVICE->CreateBuffer(&m_Desc, nullptr, m_CB.GetAddressOf())))
	{
		return E_FAIL;
	}
	return S_OK;
}


void CConstBuffer::Binding()
{
	CONTEXT->VSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());		// Type번 레지스터에 바인딩
	CONTEXT->HSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
	CONTEXT->DSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
	CONTEXT->GSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
	CONTEXT->PSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
}

void CConstBuffer::Binding_CS()
{
	CONTEXT->CSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
}


void CConstBuffer::SetData(void* _pData, UINT _DataSize)
{
	D3D11_MAPPED_SUBRESOURCE tMapSub = {};

	// GPU 메모리의 상수버퍼를 CPU에서 접근 가능한 메모리로 맵핑
	CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tMapSub);

	if (_DataSize == 0)
	{
		_DataSize = m_Desc.ByteWidth;
	}

	// CPU 메모리의 데이터를 맵핑된 GPU로 복사
	memcpy(tMapSub.pData, _pData, _DataSize);

	// 데이터 복사가 끝나면 맵핑을 해제
	CONTEXT->Unmap(m_CB.Get(), 0);

}


