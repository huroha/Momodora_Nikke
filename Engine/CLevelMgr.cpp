#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "components.h"

#include "CCollisionMgr.h"
#include "CAssetMgr.h"
#include "CRenderMgr.h"
#include "assets.h"

#include "CDevice.h"

#include "CStructuredBuffer.h"
#include "CClearColorCS.h"

#include "CFontMgr.h"

LEVEL_LOAD CLevelMgr::g_LevelLoad = nullptr;

CLevelMgr::CLevelMgr()
	: m_CurLevel(nullptr)
{

}

CLevelMgr::~CLevelMgr()
{
	DELETE(m_CurLevel);
}

void CLevelMgr::Init()
{

}

void CLevelMgr::Progress()
{
	// 1������ ������� ���� ���
	if (nullptr == m_CurLevel)
		return;

	m_CurLevel->RegisterClear();

	if (m_CurLevel->GetState() == LEVEL_STATE::PLAY || m_CurLevel->GetState() == LEVEL_STATE::PAUSE)
	{
		m_CurLevel->Tick();
	}

	m_CurLevel->FinalTick();
}

void CLevelMgr::RegisterObject(CGameObject* _Object)
{
	int LayerIdx = _Object->GetLayerIdx();
	CLayer* pLayer = m_CurLevel->GetLayer(LayerIdx);
	pLayer->RegisterObject(_Object);
}

CGameObject* CLevelMgr::FindObjectByName(const wstring& _Name)
{
	if (nullptr == m_CurLevel)
		return nullptr;

	return m_CurLevel->FindObjectByName(_Name);
}

void CLevelMgr::ChangeLevelState(LEVEL_STATE _NextState)
{
	assert(m_CurLevel);
	if (m_CurLevel->GetState() == _NextState)
		return;

	// Play or None ->  Pause 
	//			    ��> Stop	
	if (_NextState == LEVEL_STATE::PAUSE || _NextState == LEVEL_STATE::STOP)
	{
		CFontMgr::GetInst()->SetPause(true);
		CTimeMgr::GetInst()->SetStopMode(true);
		CRenderMgr::GetInst()->SetEditorMode(true);

		if (LEVEL_STATE::STOP == _NextState)
			CRenderMgr::GetInst()->DeregisterCamera();
	}

	// Pause, Stop -> Play
	else if (_NextState == LEVEL_STATE::PLAY)
	{
		CTimeMgr::GetInst()->SetStopMode(false);
		CRenderMgr::GetInst()->SetEditorMode(false);
		CFontMgr::GetInst()->SetPause(false);
	}

	// Stop -> Play
	if ((m_CurLevel->GetState() == LEVEL_STATE::STOP || m_CurLevel->GetState() == LEVEL_STATE::NONE)
		&& _NextState == LEVEL_STATE::PLAY)
	{
		m_CurLevel->ChangeState(_NextState);
		m_CurLevel->Begin();
	}
	else
	{
		m_CurLevel->ChangeState(_NextState);
	}
}

void CLevelMgr::ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _NextLevelState)
{
	CLevel* pPrevLevel = m_CurLevel;

	m_CurLevel = _NextLevel;

	ChangeLevelState(_NextLevelState);

	// ���� ������ ����
	if (nullptr != pPrevLevel)
		delete pPrevLevel;
}

int CLevelMgr::IsPauseState()
{
	if (m_CurLevel->GetState() == LEVEL_STATE::PAUSE)
		return 1;
	else
		return 0;
}
