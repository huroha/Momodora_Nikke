#pragma once
#include    <Engine/CScript.h>

class CNpcTemple :
    public CScript
{
private:
    vector<CGameObject*>    m_Child;
    CGameObject*            m_MainCamera;

    CGameObject*            m_DialogBox;

    int                     m_DialogStart;
    bool                    m_Start;
    bool                    m_Next;

public:

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    int GetDialogStartID() { return m_DialogStart; }
    void    ResetNext() { m_Next = false; }

public:
    CLONE(CNpcTemple);
    CNpcTemple();
    ~CNpcTemple();
};

