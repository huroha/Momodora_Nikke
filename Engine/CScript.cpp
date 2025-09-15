#include "pch.h"
#include "CScript.h"


CScript::CScript(UINT _ScriptType)
	: CComponent(COMPONENT_TYPE::SCRIPT)
	, m_ScriptType(_ScriptType)
{
}

CScript::~CScript()
{
}


void CScript::Instantiate(Ptr<CPrefab> _Pref, Vec3 _WorldPos, int _Layer)
{
	if (nullptr == _Pref)
		return;

	CGameObject* pNewObject = _Pref->Instantiate();

	pNewObject->Transform()->SetRelativePos(_WorldPos);

	CreateObject(pNewObject, _Layer, false);
}

CGameObject* CScript::Instantiate_Edit(Ptr<CPrefab> _Pref, Vec3 _WorldPos, int _Layer)
{
	if (nullptr == _Pref)
		return nullptr;

	CGameObject* pNewObject = _Pref->Instantiate();

	pNewObject->Transform()->SetRelativePos(_WorldPos);

	CreateObject(pNewObject, _Layer, false);

	return pNewObject;
}
