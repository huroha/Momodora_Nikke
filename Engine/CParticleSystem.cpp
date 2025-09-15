#include "pch.h"
#include "CParticleSystem.h"

#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CStructuredBuffer.h"

#include "CTransform.h"


CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLE_SYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_SpawnCountBuffer(nullptr)
	, m_ModuleBuffer(nullptr)
	, m_Time(0.f)
	, m_MaxParticleCount(1000)
{
	// Mesh / Material 
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleMtrl"));

	// ParticleTick ComputeShader
	m_TickCS = (CParticleTickCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleTickCS").Get();

	// Particle Texture
	m_ParticleTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\particle\\TX_GlowScene_2.png");

	// Particle Buffer
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticleCount, SB_TYPE::SRV_UAV, false);

	// SpawnCount Buffer
	m_SpawnCountBuffer = new CStructuredBuffer;
	m_SpawnCountBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::SRV_UAV, true, nullptr);

	// ComputeShader
	m_TickCS = (CParticleTickCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleTickCS").Get();


	// ��ƼŬ ���(���) ���� ����
	// Spawn Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
	m_Module.SpawnRate = 20;
	m_Module.vSpawnColor = Vec4(0.9f, 0.2f, 0.1f, 1.f);
	m_Module.MinLife = 1.f;
	m_Module.MaxLife = 5.f;
	m_Module.vSpawnMinScale = Vec3(50.f, 5.f, 1.f);
	m_Module.vSpawnMaxScale = Vec3(500.f, 50.f, 1.f);

	m_Module.SpawnShape = 1;
	m_Module.SpawnShapeScale.x = 800.f;

	m_Module.BlockSpawnShape = 1;
	m_Module.BlockSpawnShapeScale.x = 760.f;

	m_Module.SpaceType = 1; // Local Space 

	// Spawn Burst Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = false;
	m_Module.SpawnBurstRepeat = true;
	m_Module.SpawnBurstCount = 100;
	m_Module.SpawnBurstRepeatTime = 3.f;

	// Scale Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = true;
	m_Module.StartScale = 2.f;
	m_Module.EndScale = 2.f;

	// AddVelocity Module
	m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = false;
	m_Module.AddVelocityType = 1;
	m_Module.AddVelocityFixedDir = Vec3(0.f, 1.f, 0.f);
	m_Module.AddMinSpeed = 100.f;
	m_Module.AddMaxSpeed = 500.f;

	// Drag Module (����)
	m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] = false;
	m_Module.DestNormalizedAge = 0.2f;
	m_Module.LimitSpeed = 0.f;

	// Noise Force Module
	m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
	m_Module.NoiseForceTerm = 0.3f;
	m_Module.NoiseForceScale = 100.f;

	// Render Module
	m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
	m_Module.EndColor = Vec3(0.8f, 0.1f, 0.5f);
	m_Module.FadeOut = true;
	m_Module.FadeOutStartRatio = 0.9f;
	m_Module.VelocityAlignment = false;
	m_Module.UseCrossMesh = false;



	m_ModuleBuffer = new CStructuredBuffer;
	m_ModuleBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::SRV_UAV, true, &m_Module);
}

CParticleSystem::CParticleSystem(const CParticleSystem& _Origin)
	: CRenderComponent(_Origin)
	, m_ParticleBuffer(nullptr)
	, m_SpawnCountBuffer(nullptr)
	, m_ModuleBuffer(nullptr)
	, m_Time(0.f)
	, m_BurstTime(0.f)
	, m_MaxParticleCount(_Origin.m_MaxParticleCount)
	, m_TickCS(_Origin.m_TickCS)
	, m_ParticleTex(_Origin.m_ParticleTex)
	, m_Module(_Origin.m_Module)
{
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticleCount, SB_TYPE::SRV_UAV, false);

	m_SpawnCountBuffer = new CStructuredBuffer;
	m_SpawnCountBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::SRV_UAV, true, nullptr);

	m_ModuleBuffer = new CStructuredBuffer;
	m_ModuleBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::SRV_UAV, true, &m_Module);
}


CParticleSystem::~CParticleSystem()
{
	delete m_ParticleBuffer;
	delete m_SpawnCountBuffer;
	delete m_ModuleBuffer;
}

void CParticleSystem::CaculateSpawnCount()
{
	tSpawnCount count = {  };

	if (m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN])
	{
		m_Time += EngineDT;
		// SpawnRate �� ���� �̹� Tick ���� ������ų ��ƼŬ�� ��
		float Term = 1.f / (float)m_Module.SpawnRate;
		UINT SpawnCount = 0;

		if (Term < m_Time)
		{
			float Value = m_Time / Term;
			SpawnCount = (UINT)Value;
			m_Time -= (float)SpawnCount * Term;
		}

		count.SpawnCount = SpawnCount;
	}



	if (m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST])
	{
		UINT BurstCount = 0;

		if (0.f == m_BurstTime)
		{
			BurstCount = m_Module.SpawnBurstCount;
		}

		m_BurstTime += EngineDT;

		if (m_Module.SpawnBurstRepeat && m_Module.SpawnBurstRepeatTime <= m_BurstTime)
		{
			m_BurstTime -= m_Module.SpawnBurstRepeatTime;
			BurstCount += m_Module.SpawnBurstCount;
		}

		count.SpawnCount += BurstCount;
	}

	// SpawnCount �� Buffer �� ����	
	m_SpawnCountBuffer->SetData(&count);
}

void CParticleSystem::FinalTick()
{
	if (!GetActive())
		return;

	// SpawnCount ���
	CaculateSpawnCount();

	// ComputeShader
	m_TickCS->SetParticleWorldPos(Transform()->GetWorldPos());
	m_TickCS->SetParticleBuffer(m_ParticleBuffer);
	m_TickCS->SetSpawnCount(m_SpawnCountBuffer);
	m_TickCS->SetModuleBuffer(m_ModuleBuffer);

	m_TickCS->Execute();
}

void CParticleSystem::Render()
{
	if (!GetActive())
		return;

	// ��ġ���� ���ε�
	Transform()->Binding();

	// ��ƼŬ ���� ���ε�
	m_ParticleBuffer->Binding(16);	// t16

	// ��� ���� ���ε�
	m_ModuleBuffer->SetData(&m_Module);
	m_ModuleBuffer->Binding(17);	// t17
	// �������� ���ε�
	GetMaterial()->SetTexParam(TEX_0, m_ParticleTex);
	GetMaterial()->Binding();

	// ������
	GetMesh()->Render_Particle(m_MaxParticleCount);

	// ��ƼŬ ���� ���ε� ����
	m_ParticleBuffer->Clear(16);
	m_ModuleBuffer->Clear(17);
}

void CParticleSystem::SaveComponent(FILE* _File)
{
	SaveAssetRef(m_TickCS, _File);
	SaveAssetRef(m_ParticleTex, _File);

	fwrite(&m_Time, sizeof(float), 1, _File);
	fwrite(&m_BurstTime, sizeof(float), 1, _File);
	fwrite(&m_MaxParticleCount, sizeof(int), 1, _File);
	fwrite(&m_Module, sizeof(tParticleModule), 1, _File);
}

void CParticleSystem::LoadComponent(FILE* _File)
{
	m_TickCS = (CParticleTickCS*)LoadAssetRef<CComputeShader>(_File);
	LoadAssetRef(m_ParticleTex, _File);

	fread(&m_Time, sizeof(float), 1, _File);
	fread(&m_BurstTime, sizeof(float), 1, _File);
	fread(&m_MaxParticleCount, sizeof(int), 1, _File);
	fread(&m_Module, sizeof(tParticleModule), 1, _File);
}