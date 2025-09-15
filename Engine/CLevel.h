#pragma once

#include "CEntity.h"

#include "CLayer.h"

class CLevel
	: public CEntity
{
private:
	LEVEL_STATE	m_State;
	CLayer		m_arrLayer[MAX_LAYER];

	int			m_PauseUILayerIdx;
public:
	CLayer* GetLayer(int _Layeridx) { return &m_arrLayer[(UINT)_Layeridx]; }
	CGameObject* FindObjectByName(const wstring& _Name);
	LEVEL_STATE GetState() { return m_State; }

	void RegisterClear()
	{
		// 이전프레임에 등록받았던 오브젝트 목록을 Clear 한다.
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			m_arrLayer[i].m_vecObjects.clear();
		}
	}

	void SetPauseUILayer(int _LayerIdx) { m_PauseUILayerIdx = _LayerIdx; }
	int GetPauseUILayer() { return m_PauseUILayerIdx; }

public:
	void Begin();
	void Tick();
	void FinalTick();

private:
	void ChangeState(LEVEL_STATE _NextState);

public:
	void AddObject(int _LayerIdx, CGameObject* _Object, bool _MoveWithChild);
public:
	virtual CLevel* Clone() { return new CLevel(*this); }
	CLevel();
	~CLevel();

	friend class CLevelMgr;
};
