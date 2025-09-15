#pragma once



typedef DirectX::SimpleMath::Vector2    Vec2;
typedef DirectX::SimpleMath::Vector3    Vec3;
typedef DirectX::SimpleMath::Vector4    Vec4;
typedef DirectX::SimpleMath::Matrix     Matrix;

typedef int(*GAMEOBJECT_SAVE)(class CGameObject*, FILE*);
typedef class CGameObject* (*GAMEOBJECT_LOAD)(FILE*);

typedef class CLevel* (*LEVEL_LOAD)(const wstring&);




#define RELEASE(Inst) if(nullptr != Inst) Inst->Release();


#define DEVICE CDevice::GetInst()->GetDevice().Get()
#define CONTEXT CDevice::GetInst()->GetContext().Get()

#define DELETE(p) if(nullptr != p) {delete p; p = nullptr;}
#define DELETE_ARR(p) if(nullptr != p) {delete[] p; p = nullptr;}

#define KEY_CHECK(KEY, STATE) CKeyMgr::GetInst()->GetKeyState(KEY) == STATE
#define KEY_TAP(KEY) KEY_CHECK(KEY, KEY_STATE::TAP)
#define KEY_RELEASED(KEY) KEY_CHECK(KEY, KEY_STATE::RELEASED)
#define KEY_PRESSED(KEY) KEY_CHECK(KEY, KEY_STATE::PRESSED)

#define MAX_LAYER   32

#define DT CTimeMgr::GetInst()->GetDeltaTime()
#define EngineDT CTimeMgr::GetInst()->GetEngineDeltaTime()

#define CLONE(Type) virtual Type* Clone() { return new Type(*this); }

#define CLONE_DISABLE(Type) virtual Type* Clone() { return nullptr; }\
							Type(const Type& _Origin) = delete;