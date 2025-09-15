#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer :
	public CEntity
{
private:
	vector<CGameObject*>	m_vecParentObjects; // Layer �Ҽ� ������Ʈ && �ֻ��� �θ�
	vector<CGameObject*>	m_vecObjects;		// Layer �Ҽ� ������Ʈ
	int						m_LayerIdx;			// Level �� �����ִ� Layer �� �ε���


public:
	const vector<CGameObject*>& GetObjects() { return m_vecObjects; }

	void RegisterAsParent(CGameObject* _Object);
	void DisconnectObject(CGameObject* _Object);
	void RegisterObject(CGameObject* _Object) { m_vecObjects.push_back(_Object); }
	const vector<CGameObject*>& GetParentObjects() { return m_vecParentObjects; }

public:
	void AddObject(CGameObject* _Object, bool _MoveWithChild);

public:
	void Begin();
	void Tick();
	void FinalTick();

public:
	CLONE(CLayer);
	CLayer();
	CLayer(const CLayer& _Origin);
	~CLayer();

	friend class CLevel;
};
