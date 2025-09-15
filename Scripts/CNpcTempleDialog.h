#pragma once
#include    <Engine/CScript.h>

class CNpcTempleDialog :
    public CScript
{
private:
    int     m_Dialog_1;
    int     m_Dialog_2;
    int     m_Dialog_3;

    bool    m_Next;
    bool    m_Last;
public:

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    int GetDialog1() { return m_Dialog_1; }
    int GetDialog2() { return m_Dialog_2; }
    int GetDialog3() { return m_Dialog_3; }

public:
    CLONE(CNpcTempleDialog);
    CNpcTempleDialog();
    ~CNpcTempleDialog();
};

