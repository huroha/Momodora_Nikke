#pragma once
#include "CEntity.h"
#include "CGameObject.h"

#define GET_OTHER_COMPONENT(Type) C##Type* Type() { return m_Owner->Type(); }

class CComponent :
    public CEntity
{
private:
    CGameObject*            m_Owner;
    const COMPONENT_TYPE    m_Type;



public:
    GET_OTHER_COMPONENT(Transform);
    GET_OTHER_COMPONENT(MeshRender);
    GET_OTHER_COMPONENT(Camera);
    GET_OTHER_COMPONENT(Collider2D);
    GET_OTHER_COMPONENT(FlipbookPlayer);
    GET_OTHER_COMPONENT(TileMap);
    GET_OTHER_COMPONENT(Light2D);
    GET_OTHER_COMPONENT(ParticleSystem);
    GET_OTHER_COMPONENT(RigidBody2D);
    GET_OTHER_COMPONENT(StateMachine);

public:
    COMPONENT_TYPE GetType() { return m_Type; }
    CGameObject* GetOwner() { return m_Owner; }

public:
    virtual void Init() {}; // ������ ���� ���Ŀ�, GameObject �� AddComponent �� ���� ȣ��Ǵ� �Լ�
    virtual void Begin() {};
    virtual void Tick() {};
    virtual void FinalTick() = 0;



    virtual void SaveToLevel(FILE* _File) override;
    virtual void LoadFromLevel(FILE* _File) override;

    virtual void SaveComponent(FILE* _File) = 0;
    virtual void LoadComponent(FILE* _File) = 0;


public:
    virtual CComponent* Clone() = 0;
    CComponent(COMPONENT_TYPE _TYPE);
    ~CComponent();

    friend class CGameObject;
};
