#pragma once
#include "CComponent.h"

enum COLLIDER_STATE
{
    ACTIVE,
    SEMIDEACTIVE,
    DEACTIVE,
};


class CCollider2D :
    public CComponent
{
private:
    Vec2        m_Offset;
    Vec2        m_Scale;
    Vec2        m_FinalPos;
    Matrix      m_matColliderWorld; // 크기, 회전, 이동
    bool        m_IndependentScale;

    int         m_OverlapCount;

    int         m_State;


public:
    void SetOffset(Vec2 _Offset) { m_Offset = _Offset; }
    void SetScale(Vec2 _Scale) { m_Scale = _Scale; }

    Vec2 GetOffset() { return m_Offset; }
    Vec2 GetScale() { return m_Scale; }

    const Matrix& GetColliderWorldMat() { return m_matColliderWorld; }

    void SetIndependentScale(bool _Scale) { m_IndependentScale = _Scale; }
    bool IsIndependentScale() { return m_IndependentScale; }

    int  GetState() { return m_State; }
    bool IsActive() { return m_State == ACTIVE; }
    int  GetOverlapCount() { return m_OverlapCount; }


    // 충돌체 화성화
    void Activate();

    // 충돌체 비활성화
    void Deactivate();

public:
    virtual void FinalTick() override;

public:
    void BeginOverlap(CCollider2D* _Other);
    void Overlap(CCollider2D* _Other);
    void EndOverlap(CCollider2D* _Other);

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

public:
    CLONE(CCollider2D);
    CCollider2D();
    CCollider2D(const CCollider2D& _Origin);
    ~CCollider2D();
};
