#pragma once
#include <Engine/CScript.h>
class CGroundSlope :
    public CScript
{
private:
    float   m_SlopeAngle;       // ��簢�� (Degree)
    Vec2    m_SlopeDir;         // ������
    bool    m_IsRight;          // ���� ��� �������


public:
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    void SetSlopeAngle(float _Angle) {
        m_SlopeAngle = _Angle;
        float radian = (_Angle * XM_PI) / 180.f;
        m_SlopeDir = Vec2(cos(radian), sin(radian));
        m_IsRight = (_Angle > 0.f);
    }

public:
    CLONE(CGroundSlope);
    CGroundSlope();
    ~CGroundSlope();
};

