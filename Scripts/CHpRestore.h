#pragma once
#include <Engine/CScript.h>

class CHpRestore :
    public CScript
{
private:
    CGameObject*    m_Target;

    float           m_Time;
    bool            m_Effect;
public:
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;

    void SetTargetObject(CGameObject* _Obj) { m_Target = _Obj; }
    void SetEffect(bool _effect);

public:
    CLONE(CHpRestore);
    CHpRestore();
    ~CHpRestore();
};

