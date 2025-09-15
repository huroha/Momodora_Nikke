#include "pch.h"
#include "CTileMap.h"

#include "CAssetMgr.h"
#include "CTransform.h"
#include "CDevice.h"
#include "CStructuredBuffer.h"

CTileMap::CTileMap()
    : CRenderComponent(COMPONENT_TYPE::TILEMAP)
    , m_Col(1)
    , m_Row(1)
    , m_TileSizeX(1.f)
    , m_TileSizeY(1.f)
    , m_TileAtlasSizeX(0)
    , m_TileAtlasSizeY(0)
    , m_TileAtlasCol(0)
    , m_TileAtlasRow(0)
    , m_Buffer(nullptr)
{
    SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TileMapMtrl"));

    // ����ȭ���� ��ü ����
    m_Buffer = new CStructuredBuffer;

}

CTileMap::CTileMap(const CTileMap& _Origin)
    : CRenderComponent(_Origin)
    , m_Col(_Origin.m_Col)
    , m_Row(_Origin.m_Row)
    , m_TileSizeX(_Origin.m_TileSizeX)
    , m_TileSizeY(_Origin.m_TileSizeY)
    , m_TileAtlas(_Origin.m_TileAtlas)
    , m_TileAtlasSizeX(_Origin.m_TileAtlasSizeX)
    , m_TileAtlasSizeY(_Origin.m_TileAtlasSizeY)
    , m_TileAtlasRow(_Origin.m_TileAtlasRow)
    , m_TileAtlasCol(_Origin.m_TileAtlasCol)
    , m_vecTileInfo(_Origin.m_vecTileInfo)
{
    // ����ȭ���� ��ü ����
    m_Buffer = new CStructuredBuffer;
}

void CTileMap::Init()
{
    SetTileColRow(m_Col, m_Row);
    SetTileSize(m_TileSizeX, m_TileSizeY);

    if (nullptr != m_TileAtlas)
    {
        SetTileAtlasSize(m_TileAtlasSizeX, m_TileAtlasSizeY);
    }
}

CTileMap::~CTileMap()
{
    DELETE(m_Buffer);
}

void CTileMap::SetTileColRow(UINT _Col, UINT _Row)
{
    m_Col = _Col;
    m_Row = _Row;

    CalcObjectScale();

    // ���� �� ����ȭ���� ũ�� ����
    if (m_vecTileInfo.size() != m_Col * m_Row)
        m_vecTileInfo.resize(m_Col * m_Row);
    if (FAILED(m_Buffer->Create(sizeof(tTileInfo), m_Col * m_Row, SB_TYPE::SRV_ONLY, true)))
    {
        assert(nullptr);
    }

    // �ӽ� �ڵ�
    for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
    {
        tTileInfo info = {};


        info.SliceUV = Vec2(1.f / m_TileAtlasCol, 1.f / m_TileAtlasRow);
        //info.LeftTopUV = Vec2(info.SliceUV.x * (i % m_TileAtlasCol), info.SliceUV.y * (i / m_TileAtlasCol));
        info.LeftTopUV = Vec2(1.f / m_TileAtlasCol, 0.f);

        m_vecTileInfo[i] = info;
    }
}

void CTileMap::SetTileSize(float _TileSizeX, float _TileSizeY)
{
    m_TileSizeX = _TileSizeX;
    m_TileSizeY = _TileSizeY;

    CalcObjectScale();
}

void CTileMap::SetTileAtlasSize(UINT _X, UINT _Y)
{
    assert(nullptr != m_TileAtlas.Get());

    m_TileAtlasSizeX = _X;
    m_TileAtlasSizeY = _Y;

    SetTileAtlasColRow(m_TileAtlas->GetWidth() / m_TileAtlasSizeX, m_TileAtlas->GetHeight() / m_TileAtlasSizeY);
}

void CTileMap::SetTileAtlasColRow(UINT _Col, UINT _Row)
{
    assert(nullptr != m_TileAtlas.Get());

    m_TileAtlasCol = _Col;
    m_TileAtlasRow = _Row;

    m_TileAtlasSizeX = m_TileAtlas->GetWidth() / m_TileAtlasCol;
    m_TileAtlasSizeY = m_TileAtlas->GetHeight() / m_TileAtlasRow;
}

void CTileMap::CalcObjectScale()
{
    Transform()->SetRelativeScale(Vec3(m_Col * m_TileSizeX, m_Row * m_TileSizeY, 1.f));
}

void CTileMap::Render()
{
    //���� Atlas�� ���ٸ� return;
    if (nullptr == m_TileAtlas)
        return;
    
    // ��ġ����
    Transform()->Binding();


    
    // ���� ����
    Vec2 vLeftTopUV = Vec2((float)m_TileAtlasSizeX * 6.f, 0.f) / Vec2(m_TileAtlas->GetWidth(), m_TileAtlas->GetHeight());
    Vec2 vTileSliceUV = Vec2((float)m_TileAtlasSizeX / (float)m_TileAtlas->GetWidth(), (float)m_TileAtlasSizeY / (float)m_TileAtlas->GetHeight());
    GetMaterial()->SetTexParam(TEX_0, m_TileAtlas);
    GetMaterial()->SetScalarParam(INT_0, m_Col);
    GetMaterial()->SetScalarParam(INT_1, m_Row);
    GetMaterial()->Binding();

    // ����ȭ ���� ���ε�
    m_Buffer->SetData(m_vecTileInfo.data());
    m_Buffer->Binding(16);

    // ������
    GetMesh()->Render();

    // ���� ���ε� ����
    m_Buffer->Clear(16);
}

void CTileMap::SaveComponent(FILE* _File)
{
    fwrite(&m_Col, sizeof(int), 1, _File);
    fwrite(&m_Row, sizeof(int), 1, _File);
    fwrite(&m_TileSizeX, sizeof(float), 1, _File);
    fwrite(&m_TileSizeY, sizeof(float), 1, _File);

    SaveAssetRef(m_TileAtlas, _File);

    fwrite(&m_TileAtlasSizeX, sizeof(UINT), 1, _File);
    fwrite(&m_TileAtlasSizeY, sizeof(UINT), 1, _File);
    fwrite(&m_TileAtlasCol, sizeof(UINT), 1, _File);
    fwrite(&m_TileAtlasRow, sizeof(UINT), 1, _File);

    size_t TileCount = m_vecTileInfo.size();
    fwrite(&TileCount, sizeof(size_t), 1, _File);
    fwrite(m_vecTileInfo.data(), sizeof(tTileInfo), m_vecTileInfo.size(), _File);
}

void CTileMap::LoadComponent(FILE* _File)
{
    fread(&m_Col, sizeof(int), 1, _File);
    fread(&m_Row, sizeof(int), 1, _File);
    fread(&m_TileSizeX, sizeof(float), 1, _File);
    fread(&m_TileSizeY, sizeof(float), 1, _File);

    LoadAssetRef(m_TileAtlas, _File);

    fread(&m_TileAtlasSizeX, sizeof(UINT), 1, _File);
    fread(&m_TileAtlasSizeY, sizeof(UINT), 1, _File);
    fread(&m_TileAtlasCol, sizeof(UINT), 1, _File);
    fread(&m_TileAtlasRow, sizeof(UINT), 1, _File);

    size_t TileCount = 0;
    fread(&TileCount, sizeof(size_t), 1, _File);
    m_vecTileInfo.resize(TileCount);
    fread(m_vecTileInfo.data(), sizeof(tTileInfo), TileCount, _File);

    SetTileColRow(m_Col, m_Row);
}