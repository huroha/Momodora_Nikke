#pragma once
#include "CComponent.h"

enum class RIGIDBODY_MODE
{
    TOPVIEW,
    PLATFOMER,
};

class CRigidBody2D :
    public CComponent
{
private:

    RIGIDBODY_MODE      m_Mode;
    Vec3                m_Force;            // ���� �� (ũ�� + ����)
    Vec3                m_Velocity;         // �ӵ�    (ũ�� + ����)
    Vec3                m_GravidyVelocity;
    float               m_Mass;             // ����
    float               m_Friction;         // �������
    float               m_MaxSpeed;         // �ִ� �ӷ� ����
    float               m_GravityAccel;     // �߷°��ӵ� ũ��
    float               m_GravityMaxSpeed;  // �߷����� ���ؼ� �߻��ϴ� �ӵ��� �ִ� ����ġ
    float               m_JumpSpeed;        
    bool                m_IsMove;           // ���� ������Ʈ�� �����̴� ������ �ƴ��� üũ
    bool                m_IsGound;          // �������� ���ִ��� ����
    bool                m_UseGravity;       // �߷��� ������ ������


public:
    void AddForce(Vec3 _Force) { m_Force += _Force; }
    void SetMass(float _Mass) { m_Mass = _Mass; }
    void SetMaxSpeed(float _Max) { m_MaxSpeed = _Max; }
    void SetVelocity(Vec3 _Velocity) { m_Velocity = _Velocity; }
    void SetVelocityX(float _X) { m_Velocity.x = _X; }

    // Slope Test
    void SetVelocitySlope(float _Force) { m_Velocity.y = _Force; }

    // Ceiling
    void SetGravityVelocity(float _Y) { m_GravidyVelocity.y = _Y; }

    void AddVelociy(Vec3 _Velocity) { m_Velocity += _Velocity; }
    void SetFriction(float _Friction) { m_Friction = _Friction; }
    void SetMode(RIGIDBODY_MODE _Mode) { m_Mode = _Mode; }
    void SetGravityAccelScale(float _Accel) { m_GravityAccel = _Accel; }
    void SetMaxGravitySpeed(float _Max) { m_GravityMaxSpeed = _Max; }
    void UseGravity(bool _Use) { m_UseGravity = _Use; }
    void SetJumpSpeed(float _Speed) { m_JumpSpeed = _Speed; }

    void SetGround(bool _Ground);

    void Jump();
    void JellyJump(float _JumpSpeed);

    Vec3 GetForce() { return m_Force; }
    Vec3 GetVelocity() { return m_Velocity; }
    Vec3 GetGravityVelocity() { return m_GravidyVelocity; }
    float GetMass() { return m_Mass; }
    float GetMaxSpeed() { return m_MaxSpeed; }
    float GetFriction() { return m_Friction; }
    bool    IsGround() { return m_IsGound; }
    RIGIDBODY_MODE GetMode() { return m_Mode; }
    bool IsMove() { return m_IsMove; }
    float   GetGravityAccel() { return m_GravityAccel; }
    float   GetGravityMaxSpeed() { return m_GravityMaxSpeed; }
    bool    IsUseGravity() { return m_UseGravity; }
    float   GetJumpSpeed() { return m_JumpSpeed; }


public:
    virtual void FinalTick() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

public:
    CLONE(CRigidBody2D);
    CRigidBody2D();
    CRigidBody2D(const CRigidBody2D& _Other);
    ~CRigidBody2D();

};

