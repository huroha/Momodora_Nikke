#include "pch.h"
#include "CHpRestore.h"
#include <Engine/CLevelMgr.h>


CHpRestore::CHpRestore()
    : CScript((UINT)SCRIPT_TYPE::HPRESTORE)
    , m_Target(nullptr)
    , m_Effect(false)
    , m_Time(0.f)
{

}

CHpRestore::~CHpRestore()
{

}


void CHpRestore::SaveComponent(FILE* _File)
{

}

void CHpRestore::LoadComponent(FILE* _File)
{

}

void CHpRestore::Begin()
{

    SetTargetObject(CLevelMgr::GetInst()->FindObjectByName(L"MpRestore")); 
    m_Target->GetRenderComponent()->SetActive(false);

}

void CHpRestore::Tick()
{
    if (m_Effect)
        m_Time += DT;

    if (m_Time > 2.3f)
    {
        m_Effect = false;
        m_Target->GetRenderComponent()->SetActive(false);
        m_Time = 0.f;
    }
}

void CHpRestore::SetEffect(bool _effect)
{
    m_Effect = _effect;
    if(m_Effect)
        m_Target->GetRenderComponent()->SetActive(true);
}

