#include "pch.h"
#include "CParticleLeaf.h"


#include <Engine/CLevelMgr.h>

CParticleLeaf::CParticleLeaf()
    : CScript((UINT)SCRIPT_TYPE::PARTICLELEAF)
    , m_Target(nullptr)
{
}

CParticleLeaf::~CParticleLeaf()
{
}

void CParticleLeaf::SaveComponent(FILE* _File)
{
}

void CParticleLeaf::LoadComponent(FILE* _File)
{
}

void CParticleLeaf::Begin()
{
    SetTargetObject(CLevelMgr::GetInst()->FindObjectByName(L"Player"));
}

void CParticleLeaf::Tick()
{
    if (m_Target != nullptr)
    {
        Transform()->SetRelativePos(Vec3(m_Target->Transform()->GetRelativePos().x + 800.f, 400.f, 0.f));
    }
}

