#include "pch.h"
#include "CStateMachine.h"

#include "CState.h"
#include <StateMachine/CStateMgr.h>


CStateMachine::CStateMachine()
    : CComponent(COMPONENT_TYPE::STATEMACINE)
    , m_CurState(nullptr)
{
}

CStateMachine::CStateMachine(const CStateMachine& _Origin)
    : CComponent(_Origin)
    , m_CurState(nullptr)
{
	map<wstring, CState*>::const_iterator iter = _Origin.m_mapState.begin();
	for (; iter != _Origin.m_mapState.end(); ++iter)
	{
		CState* pCloneState = iter->second->Clone();
		AddState(iter->first, pCloneState);
	}
	if (nullptr != _Origin.m_CurState)
	{
		ChangeState(_Origin.m_CurState->GetName());
	}
}

CStateMachine::~CStateMachine()
{
	DeleteMap(m_mapState);
}


void CStateMachine::AddState(const wstring& _StateName, CState* _State)
{
	CState* pState = FindState(_StateName);
	assert(pState == nullptr);

	_State->SetName(_StateName);
	m_mapState.insert(make_pair(_StateName, _State));
	_State->m_Owner = this;
}

void CStateMachine::AddStateLoad(const wstring& _StateName, CState* _State)
{

	_State->SetName(_StateName);
	m_mapState.insert(make_pair(_StateName, _State));
	_State->m_Owner = this;
}

CState* CStateMachine::FindState(const wstring& _StateName)
{
	map<wstring, CState*>::iterator iter = m_mapState.find(_StateName);

	if (iter == m_mapState.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CStateMachine::ChangeState(const wstring& _StateName)
{
	CState* pNextState = FindState(_StateName);

	// 기존 상태를 빠져나오고(Exit)
	if (nullptr != m_CurState)
	{
		m_CurState->Exit();
	}

	// 새로운 상태를 가리키고
	m_CurState = pNextState;
	assert(m_CurState);

	// 새로운 상태로 진입(Enter) 한다.
	m_CurState->Enter();
}

void CStateMachine::RemoveState(const wstring& _StateName)
{
	map<wstring, CState*>::iterator iter = m_mapState.find(_StateName);

	if (iter != m_mapState.end())
	{
		ChangeStateNone();
		m_mapState.erase(iter);
	}
}

wstring CStateMachine::GetCurStateName()
{
	if (m_CurState != nullptr)
		return m_CurState->GetName();
	else
		return L"NONE";
}



void CStateMachine::FinalTick()
{
	if (nullptr == m_CurState)
		return;

	m_CurState->FinalTick();
}

void CStateMachine::SaveComponent(FILE* _File)
{
	size_t count = m_mapState.size();
	fwrite(&count, sizeof(size_t), 1, _File);
	map<wstring, CState*>::iterator iter = m_mapState.begin();
	for (; iter != m_mapState.end(); ++iter)
	{
		SaveWString(iter->first, _File);
	}
}

void CStateMachine::LoadComponent(FILE* _File)
{
	size_t count = 0;
	fread(&count, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < count; ++i)
	{
		wstring StateName;
		LoadWString(StateName, _File);
		AddStateLoad(StateName, CStateMgr::GetState(StateName));
	}
}
