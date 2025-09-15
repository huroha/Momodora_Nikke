#include "pch.h"
#include "CAssetMgr.h"

#include "CDevice.h"

void CAssetMgr::Init()
{
	CreateEngineMesh();
	CreateEngineTexture();
	CreateEngineGraphicShader();
	CreateEngineComputeShader();
	CreateEngineMaterial();
	CreateEngineSprite();
}

void CAssetMgr::CreateEngineMesh()
{

	Ptr<CMesh> pMesh = nullptr;

	// =========
	// PointMesh
	// =========
	Vtx v;
	UINT i = 0;

	pMesh = new CMesh(true);
	pMesh->Create(&v, 1, &i, 1);
	CAssetMgr::GetInst()->AddAsset(L"PointMesh", pMesh);



	//=================
	//  Rect Mesh 제작
	//=================

	// 4 개의 정점의 위치 설계
	// 0 -- 1
	// | \  |
	// 3 -- 2   
	Vtx arrVtx[4] = {};
	arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vUV = Vec2(0.f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);

	arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vUV = Vec2(1.f, 0.f);
	arrVtx[1].vColor = Vec4(0.f, 0.f, 1.f, 1.f);

	arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vUV = Vec2(1.f, 1.f);
	arrVtx[2].vColor = Vec4(0.f, 1.f, 0.f, 1.f);

	arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vUV = Vec2(0.f, 1.f);
	arrVtx[3].vColor = Vec4(1.f, 0.f, 1.f, 1.f);

	// 인덱스
	UINT Idx[6] = { 0 , 1 , 2, 0 , 2 , 3 };


	pMesh = new CMesh(true);
	pMesh->Create(arrVtx, 4, Idx, 6);
	CAssetMgr::GetInst()->AddAsset(L"RectMesh", pMesh);

	// ==============
	// RectMesh_Debug
	// 4 개의 정점의 위치 설계
	// 0 -- 1
	// | \  |
	// 3 -- 2   
	// ==============
	UINT DebugRectIdx[5] = { 0 , 1 , 2, 3 , 0 };

	pMesh = new CMesh(true);
	pMesh->Create(arrVtx, 4, DebugRectIdx, 5);
	CAssetMgr::GetInst()->AddAsset(L"RectMesh_Debug", pMesh);


	// ==========
	// CircleMesh
	// ==========
	vector<Vtx>		vecVtx;
	vector<UINT>	vecIdx;


	UINT Slice = 40;
	float Radius = 0.5f;
	float Theta = 0.f;

	// 중심점
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	for (UINT i = 0; i < Slice + 1; ++i)
	{
		v.vPos = Vec3(Radius * cosf(Theta), Radius * sinf(Theta), 0.f);
		v.vUV = Vec2(v.vPos.x + 0.5f, 1.f - (v.vPos.y + 0.5f));
		vecVtx.push_back(v);

		Theta += (XM_PI * 2.f) / Slice;
	}

	for (UINT i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 1);
		vecIdx.push_back(i + 2);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	CAssetMgr::GetInst()->AddAsset(L"CircleMesh", pMesh);

	// =================
	// CircleMesh_Debug
	// =================
	vecIdx.clear();
	for (size_t i = 0; i < vecVtx.size() - 1; ++i)
	{
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	CAssetMgr::GetInst()->AddAsset(L"CircleMesh_Debug", pMesh);


}
void CAssetMgr::CreateEngineTexture()
{
	// NoiseTexture
	Load<CTexture>(L"NoiseTex", L"Texture//noise//noise_03.jpg");
	Load<CTexture>(L"Particle_Ray", L"Texture//particle//ray.png");


	// PostPorcessTexture
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	CreateTexture(L"PostProcessTex", vResolution.x, vResolution.y
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
}


void CAssetMgr::CreateEngineGraphicShader()
{
	Ptr<CGraphicShader> pShader = nullptr;

	// ===========================
	// Std2DShader : 표준 2D 쉐이더
	// ===========================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);


	pShader->AddScalarParam("Test Parameter", INT_0, true);
	pShader->AddTexParam("Output Texture", TEX_0);


	CAssetMgr::GetInst()->AddAsset(L"Std2DShader", pShader);


	// ===========================
	// Std2DUIShader : 표준 2D UI 전용 쉐이더
	// ===========================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2DUI");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2DUI");

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddTexParam("Output Texture", TEX_0);


	CAssetMgr::GetInst()->AddAsset(L"Std2DUIShader", pShader);

	// ===========================
	// Std2DResourceShader : 표준 2D 리소스 뽑은거 전용 쉐이더
	// ===========================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2DAsset");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2DAsset");

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddTexParam("Output Texture", TEX_0);
	pShader->AddTexParam("Noise Texture", TEX_1);
	pShader->AddScalarParam("Boss Color Param", INT_0, true);

	CAssetMgr::GetInst()->AddAsset(L"Std2DAssetShader", pShader);

	// ================================================
	// Std2DAlphaBlendShader : 표준 알파블렌드 2D 쉐이더
	// ================================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D_AlphaBlend");
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDSState(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	pShader->AddTexParam("Output Texture", TEX_0);
	pShader->AddTexParam("Noise Texture", TEX_1);
	pShader->AddScalarParam("Blue Color Parameter", INT_0, true);
	CAssetMgr::GetInst()->AddAsset(L"Std2DAlphaBlendShader", pShader);


	// =================================
	// Std2D_PaperBurn : PaperBurn 쉐이더
	// =================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D_PaperBurn");

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	CAssetMgr::GetInst()->AddAsset(L"Std2DPaperBurnShader", pShader);


	// ==================================
	// TileMapShader : 타일맵 전용 쉐이더
	// ==================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(L"Shader\\tilemap.fx", "PS_TileMap");

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::LESS);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);
	CAssetMgr::GetInst()->AddAsset(L"TileMapShader", pShader);

	// ===========
	// PostProcess
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Post");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_Post");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"PostProcessShader", pShader);


	// ===========
	// Distortion
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Distortion");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_Distortion");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"DistortionShader", pShader);

	// ===========
	// Vortex
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Vortex");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_Vortex");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"VortexShader", pShader);


	// ===========
	// Wave
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Wave");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_Wave");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"WaveShader", pShader);

	// ===========
	// BossRoar
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_MonsterRoar");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_MonsterRoar");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"RoarShader", pShader);




	// =================================
	// DebugShapeShader : 디버그용 쉐이더
	// =================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\debug.fx", "VS_DebugShape");
	pShader->CreatePixelShader(L"Shader\\debug.fx", "PS_DebugShape");
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	CAssetMgr::GetInst()->AddAsset(L"DebugShapeShader", pShader);


	// ===========
	// Light Spread
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_LightSpread");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_LightSpread");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"PrayShader", pShader);


	// ==============
	// ParticleShader
	// ==============
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\particle.fx", "VS_Particle");
	pShader->CreateGeometryShader(L"Shader\\particle.fx", "GS_Particle");
	pShader->CreatePixelShader(L"Shader\\particle.fx", "PS_Particle");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDSState(DS_TYPE::NO_WRITE);


	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_PARTICLE);
	CAssetMgr::GetInst()->AddAsset(L"ParticleShader", pShader);


	// ===========
	// FadeShader
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Fade");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_Fade");

	pShader->SetBSState(BS_TYPE::ALPHABLEND);  // 알파블렌딩 사용
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);  // depth test/write 없음
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	CAssetMgr::GetInst()->AddAsset(L"FadeShader", pShader);

}

#include "CParticleTickCS.h"

void CAssetMgr::CreateEngineComputeShader()
{
	AddAsset<CComputeShader>(L"ParticleTickCS", new CParticleTickCS);
}



void CAssetMgr::CreateEngineMaterial()
{
	Ptr<CMaterial> pMtrl = nullptr;

	// Std2DMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"Std2DMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// Std2DAlphaBlendMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"Std2DAlphaBlendMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DAlphaBlendShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// Std2DPaperBurnMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"Std2DPaperBurnMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DPaperBurnShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// TileMapMaterial
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"TileMapMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"TileMapShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// PostProcessMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"PostProcessMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"PostProcessShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// DistortionMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"DistortionMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DistortionShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	pMtrl->SetTexParam(TEX_1, FindAsset<CTexture>(L"NoiseTex"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// VortexMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"VortexMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"VortexShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// WaveMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"WaveMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"WaveShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);


	// PrayMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"PrayMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"PrayShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// RoarMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"RoarMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"RoarShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);


	// DebugShapeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"DebugShapeMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DebugShapeShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// ParticleMtrl 	
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"ParticleMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"ParticleShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// FadeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"FadeMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"FadeShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);
}


void CAssetMgr::CreateEngineSprite()
{
	Ptr<CTexture> pScaletTex = Load<CTexture>(L"Player_Idle_R", L"Texture\\Idle_f.png");
	Ptr<CFlipbook> pFlipbook = new CFlipbook;
	Ptr<CSprite> pSprite = nullptr;

	for (int i = 0; i < 6; ++i)
	{
		pSprite = new CSprite;
		pSprite->SetAtlasTexture(pScaletTex);
		pSprite->SetLeftTop(Vec2(i * 64.f, 0.f));
		pSprite->SetSlice(Vec2(64.f, 64.f));
		pSprite->SetBackground(Vec2(120.f, 120.f));


		wchar_t szKey[255] = {};
		swprintf_s(szKey, 255, L"Sprite\\PlayerIdle_R_%d.sprite", i);

		// Sprite 등록
		AddAsset(szKey, pSprite);

		// flipbook 에 Sprite 추가
		pFlipbook->AddSprite(pSprite);

		//Sprite 저장
		pSprite->Save(CPathMgr::GetInst()->GetContentPath() +pSprite->GetKey());
	}

	// flipbook 등록
	AddAsset(L"Flipbook\\PlayerIdle_R.flip", pFlipbook);

	//// Flipbook 저장
	pFlipbook->Save(CPathMgr::GetInst()->GetContentPath() + pFlipbook->GetKey());


	


}