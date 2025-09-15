#include "pch.h"
#include "Inspector.h"
#include "ComponentUI.h"

#include <Engine/CLevelMgr.h>
#include <Engine/assets.h>

#include "ScriptUI.h"

#include "AssetUI.h"
#include "AddComponentUI.h"
#include <Engine/CTaskMgr.h>

Inspector::Inspector()
	: EditorUI("Inspector")
	, m_arrComUI{}
{
	CreateComponentUI();

	CreateAssetUI();

	SetTargetObject(nullptr);

}

Inspector::~Inspector()
{
}

void Inspector::Render_Update()
{
	// 레벨이 전환 될 때에는 타겟 오브젝트 초기화
	//if (CTaskMgr::GetInst()->IsLevelChanged())
	//{
	//	SetTargetObject(nullptr);
	//}

	if (nullptr != m_TargetObject)
	{
		if (!IsValid(m_TargetObject))
		{
			SetTargetObject(nullptr);
		}
	}
}

void Inspector::Tick()
{
}

void Inspector::SetTargetObject(CGameObject* _Target)
{
	// 자식 UI 들에게 TargetObject 를 찾아서 알려준다.
	m_TargetObject = _Target;
	m_AddComponentUI->SetTargetObject(_Target);
	if(_Target != nullptr)
		m_AddComponentUI->CheckComponent();

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComUI[i])
			continue;

		m_arrComUI[i]->SetTargetObject(m_TargetObject);
	}

	// 오브젝트가 소유한 Script 에 대응하는 ScriptUI 를 생성해서 매칭시켜준다.
	if (nullptr == m_TargetObject)
	{
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->SetScript(nullptr);
		}
	}
	else
	{
		const vector<CScript*>& vecScripts = m_TargetObject->GetScripts();

		// 오브젝트에 스크립트가 없다면 전에것들은 비활성화 처리 필요

		for (size_t i = 0; i < vecScripts.size(); ++i)
		{
			if (m_vecScriptUI.size() <= i)
			{
				m_vecScriptUI.push_back(new ScriptUI);
				AddChildUI(m_vecScriptUI.back());
			}

			m_vecScriptUI[i]->SetScript(vecScripts[i]);
		}

		for (size_t i = vecScripts.size(); i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->SetScript(nullptr);
		}
	}

	// AssetUI 는 비활성화
	if (nullptr != _Target)
	{
		SetTargetAsset(nullptr);
		m_AddComponentUI->SetActive(true);
	}
}

void Inspector::SetTargetAsset(Ptr<CAsset> _Asset)
{
	m_TargetAsset = _Asset;
	m_AddComponentUI->SetActive(false);
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		if (nullptr == m_TargetAsset || i != m_TargetAsset->GetAssetType())
			m_arrAssetUI[i]->SetActive(false);
		else
		{
			m_arrAssetUI[i]->SetAsset(m_TargetAsset);
			m_arrAssetUI[i]->SetActive(true);
		}
	}

	if (nullptr != m_TargetAsset)
	{
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (nullptr == m_arrComUI[i])
				continue;

			m_arrComUI[i]->SetActive(false);
		}


        for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
        {
            m_vecScriptUI[i]->SetScript(nullptr);
        }

	}
}
#include "componentsUI.h"




void Inspector::DeleteComponentCheck(COMPONENT_TYPE _Type)
{
	m_arrComUI[(UINT)_Type]->SetActive(false);
}

void Inspector::ReLoad()
{
	CGameObject* temp = m_TargetObject;
	m_TargetObject = nullptr;
	SetTargetObject(temp);
}


void Inspector::CreateComponentUI()
{
	// InspectorUI 의 자식UI 추가

	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = (ComponentUI*)AddChildUI(new TransformUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetChildSize(ImVec2(0.f, 150.f));
	
	m_AddComponentUI = (AddComponentUI*)AddChildUI(new AddComponentUI);
	m_AddComponentUI->SetChildSize(ImVec2(0.f, 50.f));


	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = (ComponentUI*)AddChildUI(new Collider2DUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetChildSize(ImVec2(0.f, 120.f));

	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA] = (ComponentUI*)AddChildUI(new CameraUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]->SetChildSize(ImVec2(0.f, 200.f));

	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = (ComponentUI*)AddChildUI(new Light2DUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetChildSize(ImVec2(0.f, 150.f));

	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = (ComponentUI*)AddChildUI(new MeshRenderUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]->SetChildSize(ImVec2(0.f, 150.f));

	m_arrComUI[(UINT)COMPONENT_TYPE::FLIPBOOKPLAYER] = (ComponentUI*)AddChildUI(new FlipbookPlayerUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::FLIPBOOKPLAYER]->SetChildSize(ImVec2(0.f, 300.f));


	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLE_SYSTEM] = (ComponentUI*)AddChildUI(new ParticleSystemUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLE_SYSTEM]->SetChildSize(ImVec2(0.f, 680.f));

	m_arrComUI[(UINT)COMPONENT_TYPE::RIGIDBODY2D] = (ComponentUI*)AddChildUI(new RigidBodyUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::RIGIDBODY2D]->SetChildSize(ImVec2(0.f, 300.f));

	m_arrComUI[(UINT)COMPONENT_TYPE::STATEMACINE] = (ComponentUI*)AddChildUI(new StateMachineUI);
	m_arrComUI[(UINT)COMPONENT_TYPE::STATEMACINE]->SetChildSize(ImVec2(0.f, 100.f));

}

#include "MeshUI.h"
#include "MeshDataUI.h"
#include "TextureUI.h"
#include "SoundUI.h"
#include "PrefabUI.h"
#include "FlipbookUI.h"
#include "SpriteUI.h"
#include "MaterialUI.h"
#include "GraphicShaderUI.h"
#include "ComputeShaderUI.h"

void Inspector::CreateAssetUI()
{
	m_arrAssetUI[(UINT)ASSET_TYPE::MESH] = new MeshUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::MESH_DATA] = new MeshDataUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::TEXTURE] = new TextureUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::SOUND] = new SoundUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::PREFAB] = new PrefabUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::FLIPBOOK] = new FlipbookUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::SPRITE] = new SpriteUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::MATERIAL] = new MaterialUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::GRAPHIC_SHADER] = new GraphicShaderUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::COMPUTE_SHADER] = new ComputeShaderUI;

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		AddChildUI(m_arrAssetUI[i]);
	}

}
