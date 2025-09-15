
#include "pch.h"
#include "TestLevel.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>

#include <Engine/CCollisionMgr.h>

#include <Scripts/CPlayerScript.h>
#include <Scripts/CMissileScript.h>
#include <Scripts/CCameraScript.h>



void TestLevel::CreateTestLevel()
{



	CLevel* pLevel = new CLevel;

	// 테스트 레벨을 현재 레벨로 지정
	ChangeLevel(pLevel, LEVEL_STATE::STOP);

	wstring NextLevelPath3 = CPathMgr::GetInst()->GetContentPath() + L"Level\\CopyRight.lv";
	ChangeLevel(CLevelMgr::g_LevelLoad(NextLevelPath3), LEVEL_STATE::PLAY);

	CCollisionMgr::GetInst()->CollisionCheck(0, 1);		// Player Land 와 Ground
	CCollisionMgr::GetInst()->CollisionCheck(10, 12);	// BackGround_Interacction, Player_AttackCollider
	CCollisionMgr::GetInst()->CollisionCheck(0, 5);		// Monster_Land , Ground
	CCollisionMgr::GetInst()->CollisionCheck(13, 3);	// 몬스터 Detect 와 Player_Collider
	CCollisionMgr::GetInst()->CollisionCheck(14, 12);	// 몬스터 Body 와 Player_AttackCollider
	CCollisionMgr::GetInst()->CollisionCheck(14, 3);	// 몬스터 Body 와 Player_Collider
	CCollisionMgr::GetInst()->CollisionCheck(6, 3);		// 몬스터 Attack 와 Player_Collider
	CCollisionMgr::GetInst()->CollisionCheck(9, 3);		// NPC 대화 와 Player_Collider
	CCollisionMgr::GetInst()->CollisionCheck(15, 0);	// SlopeCollider 와 Ground
	CCollisionMgr::GetInst()->CollisionCheck(16, 3);	// 천장과 와 player_body_Collider
	CCollisionMgr::GetInst()->CollisionCheck(0, 6);		// 몬스터 body와 Ground Collider 이긴한데 Bomb만 적용하는 걸 목표



	// Layer 이름 세팅
	//for (int i = 0; i < MAX_LAYER; ++i)
	//{
	//	pLevel->GetLayer(i)->SetName(StringToWString(layerNames[i]));
	//}

	return;


	//CGameObject* pObject = nullptr;

	//// MainCamera
	//pObject = new CGameObject;
	//pObject->SetName(L"MainCamera");
	//pObject->AddComponent(new CCamera);
	//pObject->AddComponent(new CCameraScript);

	//// MainCamera 설정
	//pObject->Camera()->SetPriority(0);
	//// 전체 Layer 찍음.
	//pObject->Camera()->LayerCheckAll();

	//pObject->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

	//pLevel->AddObject(0, pObject, false);

	////// 광원 오브젝트 추가
	//CGameObject* pLightObj = new CGameObject;
	//pLightObj->SetName(L"Light2D");
	//pLightObj->AddComponent(new CLight2D);

	//pLightObj->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	//pLightObj->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	//pLightObj->Light2D()->SetRadius(200.f);

	////m_CurLevel->AddObject(0, pLightObj, false);

	//
	//pLevel->AddObject(0,pLightObj, false);


	//// 광원 오브젝트 추가2
	////CGameObject* pLightObj3 = new CGameObject;
	////pLightObj3->SetName(L"Light2D");
	////pLightObj3->AddComponent(new CLight2D);

	////pLightObj3->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	////pLightObj3->Light2D()->SetLightColor(Vec3(1.f, 1.f, 0.f));
	////pLightObj3->Light2D()->SetRadius(200.f);
	////pLightObj3->Transform()->SetRelativePos(Vec3(0.f, 0.f,200.f));


	////m_CurLevel->AddObject(0, pLightObj3, false);
	////CreateObject(pLightObj3, 0, false);

	//// Player
	//CGameObject* pPlayer = new CGameObject;
	//pPlayer->SetName(L"Player");
	//pPlayer->AddComponent(new CMeshRender);
	//pPlayer->AddComponent(new CPlayerScript);
	//pPlayer->AddComponent(new CCollider2D);
	//pPlayer->AddComponent(new CFlipbookPlayer);
	//pPlayer->AddComponent(new CRigidBody2D);

	//pPlayer->RigidBody2D()->SetMode(RIGIDBODY_MODE::PLATFOMER);
	//pPlayer->RigidBody2D()->SetMass(1.f);
	//pPlayer->RigidBody2D()->SetMaxSpeed(300.f);
	//pPlayer->RigidBody2D()->SetFriction(1000.f);
	//pPlayer->RigidBody2D()->SetMaxGravitySpeed(1500.f);
	//pPlayer->RigidBody2D()->SetJumpSpeed(600.f);

	//pPlayer->Transform()->SetRelativePos(0.f, 0.f, 100.f);
	//pPlayer->Transform()->SetRelativeScale(192.f, 192.f, 1.f);
	//
	////pPlayer->StateMachine()->AddState(L"CPlayerIdleState", new CPlayerIdleState);


	//// Material 설정은 꼭 해주어야 함.
	////pPlayer->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
	//// Material -> Alphablending으로 변경
	////pPlayer->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));

	//pPlayer->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//Ptr<CMaterial> pMtlr = CAssetMgr::GetInst()->Load<CMaterial>(L"Material\\Default Material 0.mtrl", L"Material\\Default Material 0.mtrl");
	//pPlayer->MeshRender()->SetMaterial(pMtlr);

	//// 아직 Domain을 통한 분류를 하기 전이라, AlphaBlend여도 좀 이상함. 투명이 그림을 가리고있음.

	//// 텍스쳐를 Material 을 통해 설정 -> Flipbook을 쓸거면 필요없다.
	////pPlayer->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTestTex);

	//// Paper Burn 효과를 위한 인자 설정 - Meterial 을 PaperBurn으로도 설정해야함.
	////pPlayer->MeshRender()->GetMaterial()->SetTexParam(TEX_1, pNoiseTex);
	////pPlayer->MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, 0.0f);


	//// Collider2D 설정
	//// 콜라이더 좌표의 z는 계산될때 0으로 고정된다.
	//pPlayer->Collider2D()->SetIndependentScale(true);
	//pPlayer->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	//pPlayer->Collider2D()->SetScale(Vec2(64.f, 64.f));

	//// Flipbook 설정
	//pPlayer->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->Load<CFlipbook>(L"Flipbook\\PlayerIdle_R.flip", L"Flipbook\\PlayerIdle_R.flip"));
	//pPlayer->FlipbookPlayer()->AddFlipbook(1, CAssetMgr::GetInst()->Load<CFlipbook>(L"Flipbook\\player_Brake.flip", L"Flipbook\\player_Brake.flip"));



	//pPlayer->FlipbookPlayer()->Play(0, 8.f, true);

	//// Player 광원 오브젝트 추가
	//CGameObject* pPlayerLight = new CGameObject;
	//pPlayerLight->SetName(L"PlayerLight");
	//pPlayerLight->AddComponent(new CLight2D);

	//pPlayerLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	//pPlayerLight->Light2D()->SetLightColor(Vec3(0.f, 0.f, 1.f));
	//pPlayerLight->Light2D()->SetRadius(10.f);
	//pPlayerLight->Transform()->SetRelativePos(Vec3(0.f, 0.f, 150.f));

	//pPlayer->AddChild(pPlayerLight);



	//pLevel->AddObject(8, pPlayer, true);
	////CreateObject(pPlayer, 0, false);



	//// Monster
	//CGameObject* pMonster = new CGameObject;
	//pMonster->SetName(L"Monster");
	//pMonster->AddComponent(new CMeshRender);
	//pMonster->AddComponent(new CCollider2D);

	//pMonster->Transform()->SetRelativePos(Vec3(400.f, 0.f, 110.f));
	//pMonster->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	//pMonster->Collider2D()->SetIndependentScale(true);
	//pMonster->Collider2D()->SetScale(Vec2(210.f, 210.f));
	//pMonster->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pMonster->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

	////pMonster->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTex2);
	////CreateObject(pMonster, 1, false);
	//pLevel->AddObject(1, pMonster, false);


	//// Particle Object
	//CGameObject* pParticle = new CGameObject;
	//pParticle->SetName(L"Particle");
	//pParticle->AddComponent(new CParticleSystem);

	//Ptr<CTexture> pParticleTex = CAssetMgr::GetInst()->Load<CTexture>(L"Particle_01", L"Texture\\particle\\TX_GlowScene_2.png");
	//pParticle->ParticleSystem()->SetParticleTexture(pParticleTex);
	//pParticle->Transform()->SetRelativePos(Vec3(100.f, 100.f, 10.f));

	//// 이거 왜 바꿨드라?
	//// CreateObject(pParticle, 2, false);
	//pLevel->AddObject(2, pParticle, false);

	//CGameObject* pBG = new CGameObject;
	//pBG->SetName(L"BG");
	//pBG->AddComponent(new CMeshRender);

	//// Scale 의 z값이 1이 아니면 이상해짐.
	//pBG->Transform()->SetRelativePos(Vec3(0.f, 15.f, 110.f));
	//pBG->Transform()->SetRelativeScale(Vec3(1280.f, 720.f, 1.f));
	//Ptr<CTexture> pBGTex = CAssetMgr::GetInst()->Load<CTexture>(L"BGTex", L"Texture\\MatriarchShrineBG.png");
	//pBG->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pBG->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	//pBG->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pBGTex);
	//pLevel->AddObject(0, pBG, false);


	//CGameObject* pPlatform = new CGameObject;
	//pPlatform->SetName(L"Platform");
	//pPlatform->AddComponent(new CCollider2D);

	//pPlatform->Transform()->SetRelativePos(Vec3(0.f, -200.f, 110.f));
	//pPlatform->Transform()->SetRelativeScale(Vec3(1280.f, 50.f, 1.f));
	//pPlatform->Collider2D()->SetIndependentScale(true);
	//pPlatform->Collider2D()->SetScale(Vec2(1280.f, 50.f));
	//pLevel->AddObject(0, pPlatform, false);
	//////PostProcess
	////CGameObject* pPostProcess = new CGameObject;
	////pPostProcess->SetName(L"PostProcess");
	////pPostProcess->AddComponent(new CMeshRender);
	////pPostProcess->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

	////pPostProcess->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	////pPostProcess->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PrayMtrl"));

	////pPostProcess->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));



	//// CreateObject(pPostProcess, 2, false);	





}
