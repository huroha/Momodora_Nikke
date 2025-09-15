#include "pch.h"
#include "CLevel.h"

#include "CGameObject.h"

#include "CGameObject.h"
#include "CKeyMgr.h"
CLevel::CLevel()
	: m_State(LEVEL_STATE::NONE)
	, m_arrLayer{}
	, m_PauseUILayerIdx(-1)
{
	SetPauseUILayer(28);
    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        m_arrLayer[i].m_LayerIdx = i;
    }
}

CLevel::~CLevel()
{
}



void CLevel::Begin()
{
    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        m_arrLayer[i].Begin();
    }
}

void CLevel::Tick()
{

	// PAUSE 상태일 때는 Pause UI 레이어만 동작
	if (m_State == LEVEL_STATE::PAUSE)
	{
		// Pause UI 레이어만 Tick
		m_arrLayer[m_PauseUILayerIdx].Tick();

	}
	// PLAY 상태일 때는 모든 레이어 동작
	else if (m_State == LEVEL_STATE::PLAY)
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			m_arrLayer[i].Tick();
		}
	}





}

void CLevel::FinalTick()
{
	if (m_State == LEVEL_STATE::PAUSE)
	{
		// Pause 상태에서도 Pause UI 레이어는 업데이트
		m_arrLayer[m_PauseUILayerIdx].FinalTick();
	}
	else
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			m_arrLayer[i].FinalTick();
		}
	}
}



void CLevel::AddObject(int _LayerIdx, CGameObject* _Object, bool _MoveWithChild)
{
    m_arrLayer[_LayerIdx].AddObject(_Object, _MoveWithChild);
}

CGameObject* CLevel::FindObjectByName(const wstring& _Name)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecParents = m_arrLayer[i].GetParentObjects();

		static list<CGameObject*> queue;

		for (size_t j = 0; j < vecParents.size(); ++j)
		{
			queue.clear();
			queue.push_back(vecParents[j]);

			while (!queue.empty())
			{
				CGameObject* pObject = queue.front();
				queue.pop_front();

				if (pObject->GetName() == _Name)
				{
					return pObject;
				}

				const vector<CGameObject*>& vecChild = pObject->GetChild();
				for (size_t k = 0; k < vecChild.size(); ++k)
				{
					queue.push_back(vecChild[k]);
				}
			}
		}
	}

	return nullptr;
}

void CLevel::ChangeState(LEVEL_STATE _NextState)
{
	m_State = _NextState;
}