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
    Vec3                m_Force;            // 누적 힘 (크기 + 방향)
    Vec3                m_Velocity;         // 속도    (크기 + 방향)
    Vec3                m_GravidyVelocity;
    float               m_Mass;             // 질량
    float               m_Friction;         // 마찰계수
    float               m_MaxSpeed;         // 최대 속력 제한
    float               m_GravityAccel;     // 중력가속도 크기
    float               m_GravityMaxSpeed;  // 중력으로 인해서 발생하는 속도의 최대 제한치
    float               m_JumpSpeed;        
    bool                m_IsMove;           // 현재 오브젝트가 움직이는 중인지 아닌지 체크
    bool                m_IsGound;          // 지상위에 서있는지 판정
    bool                m_UseGravity;       // 중력을 적용할 것인지


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

