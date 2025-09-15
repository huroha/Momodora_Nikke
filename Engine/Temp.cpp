#include "pch.h"
#include "Temp.h"

#include "CAssetMgr.h"
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CDevice.h"

#include "CMesh.h"
#include "CGraphicShader.h"
#include "CConstBuffer.h"

#include "CGameObject.h"
#include "components.h"
#include "CPlayerScript.h"

CGameObject* g_Object_0 = nullptr;
CGameObject* g_Object_1 = nullptr;


int TempInit()
{
	g_Object_0 = new CGameObject;
	g_Object_0->AddComponent(new CMeshRender);
	g_Object_0->AddComponent(new CPlayerScript);

	g_Object_0->Transform()->SetRelativeScale(0.5f, 0.5f, 1.f);
	g_Object_0->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	g_Object_0->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicShader>(L"DefaultShader"));


	g_Object_1 = new CGameObject;
	g_Object_1->AddComponent(new CMeshRender);

	g_Object_1->Transform()->SetRelativeScale(0.1f, 0.1f, 1.f);
	g_Object_1->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	g_Object_1->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicShader>(L"DefaultShader"));

	return S_OK;
}

void TempTick()
{
	g_Object_0->Tick();
	g_Object_1->Tick();


	g_Object_0->FinalTick();
	g_Object_1->FinalTick();
}

void TempRender()
{
	// RenderTarget 및 DepthStencil 클리어
	CDevice::GetInst()->Clear();

	g_Object_0->Render();
	g_Object_1->Render();

	// RenderTarget 이미지를 Window 에 출력
	CDevice::GetInst()->Present();
}

void TempRelease()
{
	delete g_Object_0;
	delete g_Object_1;
}