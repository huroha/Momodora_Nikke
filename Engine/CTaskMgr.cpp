#include "pch.h"
#include "CTaskMgr.h"

#include "CAssetMgr.h"
#include "CAsset.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CGameObject.h"

CTaskMgr::CTaskMgr()
	: m_LevelChanged(false)
{

}

CTaskMgr::~CTaskMgr()
{

}

void CTaskMgr::Tick()
{
	m_LevelChanged = false;

	for (size_t i = 0; i < m_vecGC.size(); ++i)
	{
		delete m_vecGC[i];
		m_LevelChanged = true;
	}
	m_vecGC.clear();

	for (size_t i = 0; i < m_vecTask.size(); ++i)
	{
		const tTask& task = m_vecTask[i];

		switch (task.Type)
		{
		case TASK_TYPE::ADD_CHILD:
		{
			{
				CGameObject* pParent = (CGameObject*)task.Param0;
				CGameObject* pChild = (CGameObject*)task.Param1;

				// �������� ����
				if (nullptr == pParent)
				{
					if (pChild->GetParent())
					{
						pChild->DisconnecntWithParent();
						pChild->RegisterAsParent();
					}
				}

				// �������� �ִ�.
				else
				{
					if (pChild->GetParent())
						pChild->DisconnecntWithParent();
					else
					{
						int LayerIdx = pChild->GetLayerIdx();
						pChild->DisconnectWithLayer();
						pChild->m_LayerIdx = LayerIdx;
					}

					pParent->AddChild(pChild);
				}

				m_LevelChanged = true;
			}
		}
		break;

		case TASK_TYPE::CREATE_OBJECT:
		{
			CGameObject* pNewObject = (CGameObject*)task.Param0;
			int LayerIdx = (int)task.Param1;

			CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			pCurLevel->AddObject(LayerIdx, pNewObject, task.Param2);

			m_LevelChanged = true;

			if (pCurLevel->GetState() == LEVEL_STATE::PLAY
				|| pCurLevel->GetState() == LEVEL_STATE::PAUSE)
			{
				pNewObject->Begin();
			}
		}
		break;
		case TASK_TYPE::DELETE_OBJECT:
		{
			CGameObject* pObject = (CGameObject*)task.Param0;
			if (!pObject->IsDead())
			{
				pObject->m_Dead = true;
				m_vecGC.push_back(pObject);
			}
		}

		break;
		case TASK_TYPE::CHANGE_LEVEL:
		{
			CLevel* pNextLevel = (CLevel*)task.Param0;

			LEVEL_STATE eNextState = (LEVEL_STATE)task.Param1;

			CLevelMgr::GetInst()->ChangeLevel(pNextLevel, eNextState);

			m_LevelChanged = true;
		}
			break;
		case TASK_TYPE::CHANGE_LEVEL_STATE:
		{
			LEVEL_STATE eNextState = (LEVEL_STATE)task.Param0;
			CLevelMgr::GetInst()->ChangeLevelState(eNextState);
		}
		break;
		case TASK_TYPE::DELETE_ASSET:
		{
			Ptr<CAsset> pAsset = (CAsset*)task.Param0;
			CAssetMgr::GetInst()->DeleteAsset(pAsset->GetAssetType(), pAsset->GetKey());
		}
			break;
		case TASK_TYPE::DELETE_COMPONENT:
		{
			CComponent* pComponent = (CComponent*)task.Param0;
			CGameObject* pOwnerObject = (CGameObject*)task.Param1;
			pOwnerObject->DeleteComponent(pComponent->GetType());
		}
		break;



		}
	}

	m_vecTask.clear();
}