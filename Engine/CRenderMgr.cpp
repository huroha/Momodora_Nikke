#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CCamera.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CAssetMgr.h"
#include "CConstBuffer.h"
#include "CTimeMgr.h"
#include "CLight2D.h"
#include "CStructuredBuffer.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CKeyMgr.h"


CRenderMgr::CRenderMgr()
	: m_DbgObj(nullptr)
	, m_EditorCam(nullptr)
	, m_Light2DBuffer(nullptr)
	, m_IsEditor(false)
	, m_FadeObj(nullptr)
	, m_bFading(false)
	, m_bFadeIn(false)
	, m_bFadeInOut(false)
	, m_bFadeInDone(false)
	, m_fFadeTime(0.f)
	, m_fCurrentTime(0.f)
	, m_fAlpha(0.f)
	, m_fStayTime(0.f)
	, m_bFadeOutStart(false)
	, m_DebugRender(false)
{
	m_Light2DBuffer = new CStructuredBuffer;
	m_Light2DBuffer->Create(sizeof(tLight2DInfo), 2, SB_TYPE::SRV_ONLY, true);
}

CRenderMgr::~CRenderMgr()
{
	DELETE(m_DbgObj);
	DELETE(m_Light2DBuffer);
	DELETE(m_FadeObj);
}


void CRenderMgr::Init()
{
	// DebugRender 용 DummyObject
	m_DbgObj = new CGameObject;
	m_DbgObj->AddComponent(new CMeshRender);
	m_DbgObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl"));

	// Fade 효과용 Object
	m_FadeObj = new CGameObject;
	m_FadeObj->AddComponent(new CMeshRender);
	m_FadeObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_FadeObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"FadeMtrl"));


	// PostProcess 용 텍스쳐
	m_PostProcessTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex");
}

void CRenderMgr::Render()
{
	if (KEY_TAP(KEY::F9))
	{
		m_DebugRender ? m_DebugRender = false : m_DebugRender = true;
	}
	// 렌더링 시작
	RenderStart();

	UpdateFade();

	if (m_IsEditor)
	{
		Render_Editor();
	}
	else
	{
		Render_Play();
	}


    // DebugRender
    Render_Debug();


	// Fade Effect 렌더링
	RenderFade();
}

void CRenderMgr::RenderStart()
{
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");

	// Target Clear
	Vec4 vClearColor = Vec4(0.f, 0.f, 0.f, 0.f);
	CONTEXT->ClearRenderTargetView(pRTTex->GetRTV().Get(), vClearColor);
	CONTEXT->ClearDepthStencilView(pDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();

	// ViewPort 설정 - Window 화면 영역 지정
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	viewport.Width = vResolution.x;
	viewport.Height = vResolution.y;

	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	CONTEXT->RSSetViewports(1, &viewport);

	// Rendering 목적지를 지정
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());


	// 데이터 및 리소스 바인딩
	Binding();
}

void CRenderMgr::Binding()
{
	static CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::GLOBAL);

	g_Data.Light2DCount = (int)m_vecLight2D.size();

	pCB->SetData(&g_Data);
	pCB->Binding();
	pCB->Binding_CS();

	// 2D 광원 정보 바인딩
	static vector<tLight2DInfo> vecLight2DInfo;
	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLight2DInfo.push_back(m_vecLight2D[i]->GetLight2DInfo());
	}

	// 데이터를 받을 구조화버퍼의 크기가 모자라면, Resize 한다.
	if (m_Light2DBuffer->GetElementCount() < vecLight2DInfo.size())
	{
		m_Light2DBuffer->Create(sizeof(tLight2DInfo), (UINT)vecLight2DInfo.size(), SB_TYPE::SRV_ONLY, true);
	}

	// 광원 1개 이상인 경우 구조화 버퍼로 데이터 이동
	if (!vecLight2DInfo.empty())
	{
		m_Light2DBuffer->SetData(vecLight2DInfo.data(), vecLight2DInfo.size());
		m_Light2DBuffer->Binding(13);
	}

	m_vecLight2D.clear();
	vecLight2DInfo.clear();

}


void CRenderMgr::Render_Debug()
{
	list<tDebugShapeInfo>::iterator iter = m_DbgList.begin();

	for (; iter != m_DbgList.end(); )
	{
		if (m_DebugRender)
		{
			// 모양 설정
			switch ((*iter).Shape)
			{
			case DEBUG_SHAPE::RECT:
				m_DbgObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));
				break;
			case DEBUG_SHAPE::CIRCLE:
				m_DbgObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug"));
				break;
			case DEBUG_SHAPE::CROSS:
				break;
			case DEBUG_SHAPE::LINE:
				break;
			}

			// 위치설정
			// Wolrd Mat이 있으면 따로 구하지 않아도 됨.
			if ((*iter).matWorld == XMMatrixIdentity())
			{
				m_DbgObj->Transform()->SetRelativePos((*iter).WorldPos);
				m_DbgObj->Transform()->SetRelativeScale((*iter).Scale);
				m_DbgObj->Transform()->SetRelativeRotation((*iter).Rotation);
				m_DbgObj->Transform()->FinalTick();
			}
			else
			{
				m_DbgObj->Transform()->SetWorldMat((*iter).matWorld);
			}

			// 색상 설정
			m_DbgObj->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).Color);


			// 깊이 판정 설정
			if ((*iter).DepthTest)
			{
				m_DbgObj->MeshRender()->GetMaterial()->GetShader()->SetDSState(DS_TYPE::NO_WRITE);
			}
			else
			{
				m_DbgObj->MeshRender()->GetMaterial()->GetShader()->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
			}

			// 렌더
			m_DbgObj->Render();
		}
		

		(*iter).Time += DT;
		if ((*iter).Duration <= (*iter).Time)
			iter = m_DbgList.erase(iter);
		else
			++iter;
	}
}

void CRenderMgr::Render_Play()
{
	// 레벨 내에 카메라로 레벨 렌더링
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->SortObject();
		m_vecCam[i]->Render();
	}
}

void CRenderMgr::Render_Editor()
{
	bool isPaused = (CLevelMgr::GetInst()->GetCurrentLevel()->GetState() == LEVEL_STATE::PAUSE);

	if (isPaused)
	{
		// PAUSE 상태일 때는 Pause UI 레이어만 보이도록 임시로 레이어 마스크 저장 및 변경
		UINT prevLayerCheck = m_EditorCam->GetLayerCheck();
		m_EditorCam->LayerCheckClear();
		m_EditorCam->LayerCheck(CLevelMgr::GetInst()->GetCurrentLevel()->GetPauseUILayer());

		// 렌더링
		m_EditorCam->SortObject();
		m_EditorCam->Render();

		// 레이어 마스크 복원
		m_EditorCam->LayerCheckClear();
		m_EditorCam->LayerCheck(prevLayerCheck);
	}
	else
	{
		// 일반 에디터 모드 렌더링
		m_EditorCam->SortObject();
		m_EditorCam->Render();
	}
}




void CRenderMgr::StartFadeInOut(float _fadeTime, float _stayTime)
{
	m_bFading = true;
	m_bFadeInOut = true;
	m_bFadeInDone = false;
	m_bFadeIn = true;          // FadeIn부터 시작
	m_fFadeTime = _fadeTime;
	m_fStayTime = _stayTime;
	m_fCurrentTime = 0.f;
	m_fAlpha = 0.f;
	m_bFadeOutStart = false;
}

void CRenderMgr::UpdateFade()
{
	if (!m_bFading)
		return;

	// FadeOut 시작 플래그 초기화
	m_bFadeOutStart = false;

	m_fCurrentTime += DT;

	if (m_bFadeInOut)
	{
		if (!m_bFadeInDone)  // FadeIn 진행중
		{
			m_fAlpha = m_fCurrentTime / m_fFadeTime;

			if (m_fCurrentTime >= m_fFadeTime)
			{
				m_fAlpha = 1.f;
				m_bFadeInDone = true;
				m_fCurrentTime = 0.f;
			}
		}
		else if (m_fCurrentTime <= m_fStayTime)  // 대기시간
		{
			m_fAlpha = 1.f;
		}
		else  // FadeOut 시작
		{
			float fadeOutTime = m_fCurrentTime - m_fStayTime;

			// FadeOut이 처음 시작되는 순간 체크
			if (fadeOutTime <= DT * 5.f)  // 시간 범위를 좀 더 넓게 설정
			{
				m_bFadeOutStart = true;
			}

			m_fAlpha = 1.f - (fadeOutTime / m_fFadeTime);

			if (fadeOutTime >= m_fFadeTime)
			{
				m_bFading = false;
				m_bFadeInOut = false;
				m_fAlpha = 0.f;
			}
		}
	}
}

void CRenderMgr::RenderFade()
{
	if (!m_bFading && m_fAlpha == 0.f)
		return;

	// Fade Material의 알파값 설정
	m_FadeObj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_fAlpha);

	// 전체화면 렌더링
	m_FadeObj->Render();
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, UINT _Priority)
{
	// 등록되지 않은 카메라
	if (-1 == _Priority)
	{
		// 등록하지 않은 카메라는 지워준다.
		vector<CCamera*>::iterator iter = m_vecCam.begin();
		for (; iter != m_vecCam.end(); ++iter)
		{
			if (*iter == _Cam)
			{
				m_vecCam.erase(iter);
				return;
			}
		}
	}

	// 등록된 카메라
	else
	{
		// 우선순위에 맞게 카메라 관리 벡터 사이즈 늘려줌
		if (m_vecCam.size() <= _Priority)
		{
			m_vecCam.resize(_Priority + 1);
		}

		// 이미 카메라가 들어있으면 안됨.
		//assert(!m_vecCam[_Priority]);

		m_vecCam[_Priority] = _Cam;
	}
}

void CRenderMgr::CopyRenderTarget()
{
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	CONTEXT->CopyResource(m_PostProcessTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}


