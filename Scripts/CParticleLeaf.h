#pragma once
#include <Engine/CScript.h>
class CParticleLeaf :
    public CScript
{

private:
    CGameObject* m_Target;
public:
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;

    void SetTargetObject(CGameObject* _Obj) { m_Target = _Obj; }

public:
    CLONE(CParticleLeaf);
    CParticleLeaf();
    ~CParticleLeaf();
};

