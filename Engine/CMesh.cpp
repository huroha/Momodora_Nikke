#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"

CMesh::CMesh(bool _bEngineRes)
	: CAsset(ASSET_TYPE::MESH, _bEngineRes)
	, m_VtxSysMem(nullptr)
	, m_IdxSysMem(nullptr)
	, m_VBDesc{}
	, m_VtxCount(0)
	, m_IBDesc{}
	, m_IdxCount(0)
{
}

CMesh::~CMesh()
{
	DELETE_ARR(m_VtxSysMem);
	DELETE_ARR(m_IdxSysMem);
}

void CMesh::Binding()
{
	// �������� ���� ���� ����
	UINT Stride = sizeof(Vtx);
	UINT Offset = 0;

	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &Stride, &Offset);
	CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::Render()
{
	Binding();

	CONTEXT->DrawIndexed(m_IdxCount, 0, 0);
}

void CMesh::Render_Particle(int _Count)
{
	Binding();

	CONTEXT->DrawIndexedInstanced(m_IdxCount, _Count, 0, 0, 0);
}


int CMesh::Create(Vtx* _Vtx, UINT _VtxCount, UINT* _pIdx, UINT _IdxCount)
{
	assert(nullptr == m_VB && nullptr == m_IB);

	m_VtxCount = _VtxCount;
	m_IdxCount = _IdxCount;
	
	// GPU �ʿ��� ����� ��쿡 ����Ͽ� �ڱⰡ ����־�� ��.
	// memcpy�� ����ϴ� ����. -> �޾ƿ��� ���ڰ� ���������� ����� ����Ʈ�� ������, ���߿� ������� ��, ��������� ������ ���� ���ؼ�.
	m_VtxSysMem = new Vtx[m_VtxCount];
	memcpy(m_VtxSysMem, _Vtx, sizeof(Vtx) * m_VtxCount);
	
	m_IdxSysMem = new UINT[m_IdxCount];
	memcpy(m_IdxSysMem, _pIdx, sizeof(UINT) * m_IdxCount);


	// ���� �����͸� SysMem -> GPU Mem �� �̵�
	m_VBDesc.ByteWidth = sizeof(Vtx) * m_VtxCount;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.Usage = D3D11_USAGE_DEFAULT;

	// ������ų ������ �ʱⵥ���� ���޿�
	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = _Vtx;

	// ���ؽ� �뵵�� ID3D11Buffer ��ü ����
	if (FAILED(DEVICE->CreateBuffer(&m_VBDesc, &tSub, m_VB.GetAddressOf())))
	{
		return E_FAIL;
	}

	// �ε���
	m_IBDesc.ByteWidth = sizeof(UINT) * m_IdxCount;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.Usage = D3D11_USAGE_DEFAULT;

	// ������ų ������ �ʱⵥ���� ���޿�	
	tSub.pSysMem = _pIdx;

	// �ε��� �뵵�� ID3D11Buffer ��ü ����
	if (FAILED(DEVICE->CreateBuffer(&m_IBDesc, &tSub, m_IB.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}
