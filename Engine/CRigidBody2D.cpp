#include "pch.h"
#include "CRigidBody2D.h"
#include "CGameObject.h"
#include "CTimeMgr.h"
#include "components.h"

CRigidBody2D::CRigidBody2D()
	: CComponent(COMPONENT_TYPE::RIGIDBODY2D)
	, m_Mode(RIGIDBODY_MODE::PLATFOMER)
	, m_Mass(1.f)
	, m_MaxSpeed(0.f)
	, m_IsMove(false)
	, m_Friction(0.f)
	, m_UseGravity(true)
	, m_GravityAccel(1500.f)
	, m_GravityMaxSpeed(3000.f)
	, m_IsGound(false)
	, m_JumpSpeed(300.f)
{
}

CRigidBody2D::CRigidBody2D(const CRigidBody2D& _Other)
	: CComponent(_Other)
	, m_Mode(_Other.m_Mode)
	, m_Mass(_Other.m_Mass)
	, m_Friction(_Other.m_Friction)
	, m_MaxSpeed(_Other.m_MaxSpeed)
	, m_GravityAccel(_Other.m_GravityAccel)
	, m_GravityMaxSpeed(_Other.m_GravityMaxSpeed)
	, m_JumpSpeed(_Other.m_JumpSpeed)
	, m_IsMove(false)
	, m_IsGound(false)
	, m_UseGravity(_Other.m_UseGravity)

{
}

CRigidBody2D::~CRigidBody2D()
{
}


void CRigidBody2D::FinalTick()
{

	// ���ӵ� ���ϱ�
	// F == M x A
	// F / M == A
	Vec3 Accel = m_Force / m_Mass;

	// ���ӵ� �������� ���ӵ��� ũ�⸸ŭ �ӵ��� ������
	m_Velocity += Accel * DT;

	// �ִ�ӷ� üũ, 0.f == m_MaxSpeed ==> �ӷ� ������ ����
	if (0.f != m_MaxSpeed)
	{
		if (m_MaxSpeed < m_Velocity.Length())
		{
			m_Velocity.Normalize();
			m_Velocity *= m_MaxSpeed;
		}
	}

	// �־��� ���� ���ٸ�, �ݴ�������� �������� �ش�.
	if (m_Force == Vec3(0.f, 0.f,0.f) && m_Velocity != Vec3(0.f, 0.f,0.f))
	{
		Vec3 vFriction = -m_Velocity;
		vFriction.Normalize();
		vFriction *= m_Friction * m_Mass;
		vFriction *= DT;

		if (m_Velocity.Length() < vFriction.Length())
		{
			m_Velocity = Vec3(0.f, 0.f,0.f);
		}
		else
		{
			m_Velocity += vFriction;
		}
	}


	Vec3 vFinalVelocity = m_Velocity;

	// RigidBody �� Plarformer ����ΰ��
	if (m_Mode == RIGIDBODY_MODE::PLATFOMER)
	{
		// �߷±�� On
		if (m_UseGravity && !m_IsGound)
		{
			m_GravidyVelocity += Vec3(0.f, -1.f,0.f) * m_GravityAccel * DT;
			if (m_GravityMaxSpeed < m_GravidyVelocity.Length())
			{
				m_GravidyVelocity.Normalize();
				m_GravidyVelocity *= m_GravityMaxSpeed;
			}

			vFinalVelocity += m_GravidyVelocity;
		}
		// �߷±�� Off
		else
		{
			m_GravidyVelocity = Vec3(0.f, 0.f,0.f);
		}
	}


	// �ӵ��� ���� ��ü�� �̵� �߻�
	Vec3 vObjPos = GetOwner()->Transform()->GetRelativePos();
	GetOwner()->Transform()->SetRelativePos(vObjPos + vFinalVelocity * DT);

	// ������Ʈ�� ������ üũ
	if (vObjPos == GetOwner()->Transform()->GetRelativePos())
		m_IsMove = false;
	else
		m_IsMove = true;

	// �̹��� ���� ���� 0 ���� �ʱ�ȭ
	m_Force = Vec3(0.f, 0.f,0.f);
}



void CRigidBody2D::SaveComponent(FILE* _File)
{

	fwrite(&m_Mode, sizeof(RIGIDBODY_MODE), 1, _File);
	fwrite(&m_Force, sizeof(Vec3), 1, _File);
	fwrite(&m_Velocity, sizeof(Vec3), 1, _File);
	fwrite(&m_GravidyVelocity, sizeof(Vec3), 1, _File);
	fwrite(&m_Mass, sizeof(float), 1, _File);
	fwrite(&m_Friction, sizeof(float), 1, _File);
	fwrite(&m_MaxSpeed, sizeof(float), 1, _File);
	fwrite(&m_GravityMaxSpeed, sizeof(float), 1, _File);
	fwrite(&m_JumpSpeed, sizeof(float), 1, _File);
	fwrite(&m_IsMove, sizeof(bool), 1, _File);
	fwrite(&m_IsGound, sizeof(bool), 1, _File);
	fwrite(&m_UseGravity, sizeof(bool), 1, _File);

}

void CRigidBody2D::LoadComponent(FILE* _File)
{

	fread(&m_Mode, sizeof(RIGIDBODY_MODE), 1, _File);
	fread(&m_Force, sizeof(Vec3), 1, _File);
	fread(&m_Velocity, sizeof(Vec3), 1, _File);
	fread(&m_GravidyVelocity, sizeof(Vec3), 1, _File);
	fread(&m_Mass, sizeof(float), 1, _File);
	fread(&m_Friction, sizeof(float), 1, _File);
	fread(&m_MaxSpeed, sizeof(float), 1, _File);
	fread(&m_GravityMaxSpeed, sizeof(float), 1, _File);
	fread(&m_JumpSpeed, sizeof(float), 1, _File);
	fread(&m_IsMove, sizeof(bool), 1, _File);
	fread(&m_IsGound, sizeof(bool), 1, _File);
	fread(&m_UseGravity, sizeof(bool), 1, _File);
}


// Platform ������Ʈ�� begin overlap���� player�� setGround�� true üũ�ϴ� ����̾���.
void CRigidBody2D::SetGround(bool _Ground)
{
	m_IsGound = _Ground;

	if (m_IsGound)
	{
		m_GravidyVelocity = Vec3(0.f, 0.f, 0.f);
	}
}

void CRigidBody2D::Jump()
{
	m_GravidyVelocity += (Vec3(0.f, 1.f,0.f) * m_JumpSpeed);
	m_IsGound = false;
}

void CRigidBody2D::JellyJump(float _JumpSpeed)
{
	m_GravidyVelocity = Vec3(0.f, 0.f, 0.f);
	m_GravidyVelocity += (Vec3(0.f, 1.f, 0.f) * _JumpSpeed);
	m_IsGound = false;
}
