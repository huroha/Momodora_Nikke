#include "pch.h"
#include "ParticleSystemUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CStructuredBuffer.h>

#include <Engine/CAssetMgr.h>
#include "ListUI.h"
#include "CImGuiMgr.h"


#include "Inspector.h"

ParticleSystemUI::ParticleSystemUI()
    : ComponentUI("ParticleSystemUI", COMPONENT_TYPE::PARTICLE_SYSTEM)
{

}

ParticleSystemUI::~ParticleSystemUI()
{
}


void ParticleSystemUI::Render_Update()
{
    ComponentTitle("ParticleSystem");

	CParticleSystem* pParticle = GetTargetObject()->ParticleSystem();

	ImGui::SameLine(350);
	if (ImGui::Button("X"))
	{
		DeleteComponentTask(pParticle);

		Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
		pInspector->DeleteComponentCheck(GetType());
	}

	Ptr<CTexture> pParticeTex = GetTargetObject()->ParticleSystem()->GetParticleTex();

	string ParticleTexName;

	if (nullptr == pParticeTex)
		ParticleTexName = "None";
	else
	{
		ParticleTexName = string(pParticeTex->GetKey().begin(), pParticeTex->GetKey().end());
	}


    ImGui::Text("ParticleTex");
    ImGui::SameLine(120);
    ImGui::SetNextItemWidth(120);
	ImGui::InputText("##ParticleName", (char*)ParticleTexName.c_str(), ParticleTexName.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	ImGui::SameLine();
	if (ImGui::Button("##ParticleTexBtn", ImVec2(18.f, 18.f)))
	{
		// ListUI 를 활성화 시키기
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->SetName("ParticleTex");
		pListUI->SetActive(true);

		// ListUI 에 넣어줄 문자열 정보 가져오기
		pListUI->AddItem("None");

		vector<wstring> vecAssetNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecAssetNames);
		pListUI->AddItem(vecAssetNames);

		// 더블 클릭 시 호출시킬 함수 등록
		pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&ParticleSystemUI::SelectParticleTex);
	}

	//한칸 내리기
	ImGui::Spacing();


	tParticleModule&  pModuleStruct = GetTargetObject()->ParticleSystem()->GetModuleStucture();
	CStructuredBuffer* pModuleBuffer = GetTargetObject()->ParticleSystem()->GetModuleBuffer();



	// SpawnModule
	ImGui::Spacing();

	static bool SpawnModulecheck = pModuleStruct.Module[(UINT)PARTICLE_MODULE::SPAWN];
	if (ImGui::Checkbox("##Module1", &SpawnModulecheck))
	{
		if (!SpawnModulecheck)
		{
			pModuleStruct.Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
		}
		else
		{
			pModuleStruct.Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
		}
	}
	ImGui::SameLine();
	ImGui::SeparatorText("Spawn Module");
	// Spawn이 체크 되어있으면 활성화할것
	if (pModuleStruct.Module[(UINT)PARTICLE_MODULE::SPAWN])
	{
		ImGui::Text("SpawnRate");
		ImGui::SameLine(120);
		int spawnrate = pModuleStruct.SpawnRate;
		if (ImGui::DragInt("##SpawnRate", &spawnrate, 0.5f))
		{
			if (spawnrate < 0)
				spawnrate = 0;
			pModuleStruct.SpawnRate = spawnrate;
		}



		ImGui::Text("SpawnColor");
		ImGui::SameLine(120);
		Vec4 color = pModuleStruct.vSpawnColor;
		static bool alpha_preview = true;
		static bool alpha_half_preview = false;
		static bool drag_and_drop = true;
		static bool options_menu = true;
		static bool hdr = false;

		ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
		if (ImGui::ColorEdit3("##SpawnColor", (float*)&color, misc_flags))
		{
			pModuleStruct.vSpawnColor = Vec4(color);
		}

		ImGui::Text("MinLife");
		ImGui::SameLine(120);
		float minLife = pModuleStruct.MinLife;
		if (ImGui::DragFloat("##MinLife", &minLife, 0.1f))
		{
			if (minLife < 0)
				minLife = 0.f;
			pModuleStruct.MinLife = minLife;
		}

		ImGui::Text("MaxLife");
		ImGui::SameLine(120);
		float maxLife = pModuleStruct.MaxLife;
		if (ImGui::DragFloat("##MaxLife", &maxLife, 0.1f))
		{
			if (maxLife < 0)
				maxLife = 0.f;
			pModuleStruct.MaxLife = maxLife;
		}

		ImGui::Text("SpawnMinScale");
		ImGui::SameLine(120);
		float SpawnMinScale[3] = { pModuleStruct.vSpawnMinScale.x, pModuleStruct.vSpawnMinScale.y, pModuleStruct.vSpawnMinScale.z};
		if (ImGui::DragFloat3("##SpawnMinScale", SpawnMinScale, 0.1f,0.0f))
		{
			pModuleStruct.vSpawnMinScale.x = SpawnMinScale[0];
			pModuleStruct.vSpawnMinScale.y = SpawnMinScale[1];
			pModuleStruct.vSpawnMinScale.z = SpawnMinScale[2];
		}

		ImGui::Text("SpawnMaxScale");
		ImGui::SameLine(120);
		float SpawnMaxScale[3] = { pModuleStruct.vSpawnMaxScale.x, pModuleStruct.vSpawnMaxScale.y, pModuleStruct.vSpawnMaxScale.z };
		if (ImGui::DragFloat3("##SpawnMaxScale", SpawnMaxScale, 0.1f, 0.0f))
		{
			pModuleStruct.vSpawnMaxScale.x = SpawnMaxScale[0];
			pModuleStruct.vSpawnMaxScale.y = SpawnMaxScale[1];
			pModuleStruct.vSpawnMaxScale.z = SpawnMaxScale[2];
		}

		ImGui::Text("SpawnShape");
		ImGui::SameLine(120);
		const char* szSpawnShape[2] = { "BOX", "SPEAR" };
		int SpawnShape = pModuleStruct.SpawnShape;
		int BlockSpawnShape = pModuleStruct.BlockSpawnShape;

		if (ImGui::Combo("##comboSpawnShape", &SpawnShape, szSpawnShape, 2))
		{
			pModuleStruct.SpawnShape = SpawnShape;
			pModuleStruct.BlockSpawnShape = SpawnShape;
		}

		ImGui::Text("SpawnShapeScale");
		ImGui::SameLine(120);
		// BOX일 때
		if (pModuleStruct.SpawnShape == 0)
		{

			float SpawnShapeScale[3] = { pModuleStruct.SpawnShapeScale.x, pModuleStruct.SpawnShapeScale.y, pModuleStruct.SpawnShapeScale.z };
			if (ImGui::DragFloat3("##SpawnShapeScale1", SpawnShapeScale, 1.f, 0.0f))
			{
				pModuleStruct.SpawnShapeScale.x = SpawnShapeScale[0];
				pModuleStruct.SpawnShapeScale.y = SpawnShapeScale[1];
				pModuleStruct.SpawnShapeScale.z = SpawnShapeScale[2];
			}
		}
		else
		{

			float SpawnShapeScale = pModuleStruct.SpawnShapeScale.x;
			if (ImGui::DragFloat("##SpawnShapeScale2", &SpawnShapeScale, 1.f))
			{
				if (SpawnShapeScale < 0)
					SpawnShapeScale = 0.f;
				pModuleStruct.SpawnShapeScale.x = SpawnShapeScale;
			}
		}
		ImGui::Text("BlockSpawnShape");
		ImGui::SameLine(120);
		const char* szBlockSpawnShape[2] = { "BOX", "SPEAR" };
		if (ImGui::Combo("##comboBlockSpawnShape", &BlockSpawnShape, szBlockSpawnShape, 2))
		{
			pModuleStruct.BlockSpawnShape = BlockSpawnShape;
			pModuleStruct.SpawnShape = BlockSpawnShape;
		}

		ImGui::Text("BlockSpawnShapeScale");
		ImGui::Spacing();
		ImGui::SameLine(120);
		// BOX일 때
		if (pModuleStruct.BlockSpawnShape == 0)
		{
			float BlockSpawnShapeScale[3] = { pModuleStruct.BlockSpawnShapeScale.x, pModuleStruct.BlockSpawnShapeScale.y, pModuleStruct.BlockSpawnShapeScale.z };
			if (ImGui::DragFloat3("##BlockSpawnShapeScale1", BlockSpawnShapeScale, 1.f, 0.0f))
			{
				pModuleStruct.BlockSpawnShapeScale.x = BlockSpawnShapeScale[0];
				pModuleStruct.BlockSpawnShapeScale.y = BlockSpawnShapeScale[1];
				pModuleStruct.BlockSpawnShapeScale.z = BlockSpawnShapeScale[2];
			}
		}
		else
		{
			float BlockSpawnShapeScale = pModuleStruct.BlockSpawnShapeScale.x;
			if (ImGui::DragFloat("##BlockSpawnShapeScale2", &BlockSpawnShapeScale, 1.f))
			{
				if (BlockSpawnShapeScale < 0)
					BlockSpawnShapeScale = 0.f;
				pModuleStruct.BlockSpawnShapeScale.x = BlockSpawnShapeScale;
			}
		}


		ImGui::Text("SpaceType");
		ImGui::SameLine(120);
		const char* szSpaceType[2] = { "Local", "World" };
		int SpaceType = pModuleStruct.SpaceType;
		if (ImGui::Combo("##comboSpaceType", &SpaceType, szSpaceType, 2))
		{
			pModuleStruct.SpaceType = SpaceType;
		}

	}


	////////////////////
	// SpawnBustModule
	///////////////////

	ImGui::Spacing();


	static bool BustModulecheck = pModuleStruct.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST];
	if (ImGui::Checkbox("##Module2", &BustModulecheck))
	{
		if (!BustModulecheck)
			pModuleStruct.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = false;
		else
			pModuleStruct.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = true;
	}
	ImGui::SameLine();
	ImGui::SeparatorText("Spawn Bust Module");


	if (pModuleStruct.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST])
	{
		ImGui::Spacing();
		ImGui::Text("SpawnBurstCount");
		ImGui::SameLine(120);
		int spawnburstcount = pModuleStruct.SpawnBurstCount;
		if (ImGui::DragInt("##BustSpawnRate", &spawnburstcount, 0.5f))
		{
			if (spawnburstcount < 0)
				spawnburstcount = 0;
			pModuleStruct.SpawnBurstCount = spawnburstcount;
		}



		ImGui::Text("SpawnBurstRepeat");
		ImGui::SameLine(120);
		static bool BustRepeat = pModuleStruct.SpawnBurstRepeat;
		if (ImGui::Checkbox("##BustRepeat", &BustRepeat))
		{
			if (!BustRepeat)
				pModuleStruct.SpawnBurstRepeat = false;
			else
				pModuleStruct.SpawnBurstRepeat = true;
		}


		ImGui::Text("SpawnBurstRepeatTime");
		ImGui::Spacing();
		ImGui::SameLine(120);
		float spawnbustrepeattime = pModuleStruct.SpawnBurstRepeatTime;
		if (ImGui::DragFloat("##SpawnBurstRepeatTime", &spawnbustrepeattime, 0.1f))
		{
			if (spawnbustrepeattime < 0)
				spawnbustrepeattime = 0.f;
			pModuleStruct.SpawnBurstRepeatTime = spawnbustrepeattime;
		}



	}

	/////////////////
	// Scale Module
	/////////////////
	ImGui::Spacing();


	static bool ScaleModulecheck = pModuleStruct.Module[(UINT)PARTICLE_MODULE::SCALE];
	if (ImGui::Checkbox("##Module3", &ScaleModulecheck))
	{
		if (!ScaleModulecheck)
			pModuleStruct.Module[(UINT)PARTICLE_MODULE::SCALE] = false;
		else
			pModuleStruct.Module[(UINT)PARTICLE_MODULE::SCALE] = true;
	}
	ImGui::SameLine();
	ImGui::SeparatorText("Scale Module");

	if (pModuleStruct.Module[(UINT)PARTICLE_MODULE::SCALE])
	{

		ImGui::Text("ScaleType");
		ImGui::SameLine(120);
		const char* szScaleType[2] = { "Simple", "Advanced" };
		int scaleType = pModuleStruct.ScaleType;
		if (ImGui::Combo("##comboScaleType", &scaleType, szScaleType, 2))
		{
			pModuleStruct.ScaleType = scaleType;
		}
		if (0 == pModuleStruct.ScaleType)
		{
			ImGui::Text("StartScale");
			ImGui::SameLine(120);
			float startscale = pModuleStruct.StartScale;
			if (ImGui::DragFloat("##StartScale", &startscale, 0.1f))
			{
				if (startscale < 0)
					startscale = 0.f;
				pModuleStruct.StartScale = startscale;
			}



			ImGui::Text("EndScale");
			ImGui::SameLine(120);
			float endscale = pModuleStruct.EndScale;
			if (ImGui::DragFloat("##EndScale", &endscale, 0.1f))
			{
				if (endscale < 0)
					endscale = 0.f;
				pModuleStruct.EndScale = endscale;
			}
		}

		else
		{
			ImGui::Text("Scale X");
			ImGui::SameLine(120);
			float xScale = pModuleStruct.ScaleX;
			if (ImGui::DragFloat("##xScale", &xScale, 0.1f))
			{
				if (xScale < 0)
					xScale = 0.f;
				pModuleStruct.ScaleX = xScale;
			}

			ImGui::Text("Scale Y");
			ImGui::SameLine(120);
			float yScale = pModuleStruct.ScaleY;
			if (ImGui::DragFloat("##yScale", &yScale, 0.1f))
			{
				if (yScale < 0)
					yScale = 0.f;
				pModuleStruct.ScaleY = yScale;
			}
		}
		




		

	}


	////////////////
	// AddVelocity 
	////////////////
	ImGui::Spacing();
	static bool AddVelocityModulecheck = pModuleStruct.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY];
	if (ImGui::Checkbox("##Module4", &AddVelocityModulecheck))
	{
		if (!AddVelocityModulecheck)
			pModuleStruct.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = false;
		else
			pModuleStruct.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
	}
	ImGui::SameLine();
	ImGui::SeparatorText("Add Velocity Module");


	if (pModuleStruct.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY])
	{
		int Addvelocity = pModuleStruct.AddVelocityType;
		ImGui::Spacing();
		ImGui::Text("VelocityType");
		ImGui::SameLine(120);
		const char* szAddVelocityType[4] = { "Random", "FromCenter", "ToCenter","Fixed"};
		if (ImGui::Combo("##comboAddvelocityType", &Addvelocity, szAddVelocityType, 4))
		{
			pModuleStruct.AddVelocityType = Addvelocity;
		}

		ImGui::Text("VelocityFixedDir");
		ImGui::SameLine(120);
		float addVelocityFixedDir[3] = { pModuleStruct.AddVelocityFixedDir.x, pModuleStruct.AddVelocityFixedDir.y, pModuleStruct.AddVelocityFixedDir.z };
		if (ImGui::DragFloat3("##addVelocityFixedDir", addVelocityFixedDir, 1.f, 0.0f))
		{
			pModuleStruct.AddVelocityFixedDir.x = addVelocityFixedDir[0];
			pModuleStruct.AddVelocityFixedDir.y = addVelocityFixedDir[1];
			pModuleStruct.AddVelocityFixedDir.z = addVelocityFixedDir[2];
		}


		ImGui::Text("AddMinSpeed");
		ImGui::SameLine(120);
		float addMinSpeed = pModuleStruct.AddMinSpeed;
		if (ImGui::DragFloat("##MinSpeed", &addMinSpeed, 0.1f))
		{
			if (addMinSpeed < 0)
				addMinSpeed = 0.f;
			pModuleStruct.AddMinSpeed = addMinSpeed;
		}

		ImGui::Text("AddMaxSpeed");
		ImGui::SameLine(120);
		float addMaxSpeed = pModuleStruct.AddMaxSpeed;
		if (ImGui::DragFloat("##MaxSpeed", &addMaxSpeed, 0.1f))
		{
			if (addMaxSpeed < 0)
				addMaxSpeed = 0.f;
			pModuleStruct.AddMaxSpeed = addMaxSpeed;
		}


	}
	////////////////
	// Drag (감속) 
	////////////////
	ImGui::Spacing();
	static bool dragModulecheck = pModuleStruct.Module[(UINT)PARTICLE_MODULE::DRAG];
	if (ImGui::Checkbox("##Module5", &dragModulecheck))
	{
		if (!dragModulecheck)
			pModuleStruct.Module[(UINT)PARTICLE_MODULE::DRAG] = false;
		else
			pModuleStruct.Module[(UINT)PARTICLE_MODULE::DRAG] = true;
	}
	ImGui::SameLine();
	ImGui::SeparatorText("Drag Module");


	if (pModuleStruct.Module[(UINT)PARTICLE_MODULE::DRAG])
	{
		// 1을 준다면 생성시점부터 사라질때까지 옵션을 받겠다는 것이고,
		// 0에 가까울 수록 소멸 시점부터 옵션을 받음. 즉 0이면 옵션사용을 안하겠다는 뜻.
		ImGui::Spacing();
		ImGui::Text("StartDrag RemainAge");
		ImGui::Spacing();
		ImGui::SameLine(120);
		float normalizedAge = pModuleStruct.DestNormalizedAge;
		if (ImGui::DragFloat("##DragStartRemainAge", &normalizedAge, 0.01f))
		{
			if (normalizedAge < 0)
				normalizedAge = 0.f;
			else if (normalizedAge > 1.f)
				normalizedAge = 1.f;
			pModuleStruct.DestNormalizedAge = normalizedAge;
		}

		// 감속 최종 목표 스피드
		ImGui::Text("Dest Speed");
		ImGui::SameLine(120);
		float limitspeed = pModuleStruct.LimitSpeed;
		if (ImGui::DragFloat("##Dest Speed", &limitspeed, 0.1f))
		{
			if (limitspeed < 0)
				limitspeed = 0.f;
			pModuleStruct.LimitSpeed = limitspeed;
		}

	}

	//////////////////////
	// Noise Force Module 
	//////////////////////
	ImGui::Spacing();
	static bool NoiseForceModulecheck = pModuleStruct.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE];
	if (ImGui::Checkbox("##Module6", &NoiseForceModulecheck))
	{
		if (!NoiseForceModulecheck)
			pModuleStruct.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
		else
			pModuleStruct.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = true;
	}
	ImGui::SameLine();
	ImGui::SeparatorText("Noise Force Module");


	if (pModuleStruct.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE])
	{
		// 랜덤한 방향으로 힘을 주려고 할때 시간 빈도
		ImGui::Text("NoiseForceTerm");
		ImGui::SameLine(120);
		float noiseForceTerm = pModuleStruct.NoiseForceTerm;
		if (ImGui::DragFloat("##NoiseForceTerm", &noiseForceTerm, 0.1f))
		{
			if (noiseForceTerm < 0)
				noiseForceTerm = 0.f;
			pModuleStruct.NoiseForceTerm = noiseForceTerm;
		}

		// 힘의 크기
		ImGui::Text("NoiseForceScale");
		ImGui::SameLine(120);
		float noiseForceScale = pModuleStruct.NoiseForceScale;
		if (ImGui::DragFloat("##NoiseForceScale", &noiseForceScale, 0.1f))
		{
			if (noiseForceScale < 0)
				noiseForceScale = 0.f;
			pModuleStruct.NoiseForceScale = noiseForceScale;
		}

	}


	//////////////////////
	// Noise Force Module 
	//////////////////////
	ImGui::Spacing();
	static bool RenderModulecheck = pModuleStruct.Module[(UINT)PARTICLE_MODULE::RENDER];
	if (ImGui::Checkbox("##Module7", &RenderModulecheck))
	{
		if (!RenderModulecheck)
			pModuleStruct.Module[(UINT)PARTICLE_MODULE::RENDER] = false;
		else
			pModuleStruct.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
	}
	ImGui::SameLine();
	ImGui::SeparatorText("Render Module");


	if (pModuleStruct.Module[(UINT)PARTICLE_MODULE::RENDER])
	{
		ImGui::Spacing();
		ImGui::Text("EndColor");
		ImGui::SameLine(120);
		Vec3 color = pModuleStruct.EndColor;
		static bool alpha_preview2 = true;
		static bool alpha_half_preview2 = false;
		static bool drag_and_drop2 = true;
		static bool options_menu2 = true;
		static bool hdr2 = false;

		ImGuiColorEditFlags misc_flags2 = (hdr2 ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop2 ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview2 ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview2 ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu2 ? 0 : ImGuiColorEditFlags_NoOptions);
		if (ImGui::ColorEdit3("##EndColor", (float*)&color, misc_flags2))
		{
			pModuleStruct.EndColor = Vec3(color);
		}

		ImGui::Text("FadeOut");
		ImGui::SameLine(120);
		static bool FadeOut = pModuleStruct.FadeOut;
		if (ImGui::Checkbox("##FadeOut", &FadeOut))
		{
			if (!FadeOut)
				pModuleStruct.FadeOut = false;
			else
				pModuleStruct.FadeOut = true;
		}

		// FadeOut 효과가 시작되는 Normalized Age 지점
		ImGui::Text("FadeOutStartRatio");
		ImGui::SameLine(120);
		float FadeOutStartRatio = pModuleStruct.FadeOutStartRatio;
		if (ImGui::DragFloat("##FadeOutStartRatio", &FadeOutStartRatio, 0.01f))
		{
			if (FadeOutStartRatio < 0)
				FadeOutStartRatio = 0.f;
			else if (FadeOutStartRatio > 1.f)
				FadeOutStartRatio = 1.f;

			pModuleStruct.FadeOutStartRatio = FadeOutStartRatio;
		}


		ImGui::Text("VelocityAlignment");
		ImGui::SameLine(120);
		static bool VelocityAlignment = pModuleStruct.VelocityAlignment;
		if (ImGui::Checkbox("##VelocityAlignment", &VelocityAlignment))
		{
			if (!VelocityAlignment)
				pModuleStruct.VelocityAlignment = false;
			else
				pModuleStruct.VelocityAlignment = true;
		}

		ImGui::Text("UseCrossMesh");
		ImGui::SameLine(120);
		static bool UseCrossMesh = pModuleStruct.UseCrossMesh;
		if (ImGui::Checkbox("##UseCrossMesh", &UseCrossMesh))
		{
			if (!UseCrossMesh)
				pModuleStruct.UseCrossMesh = false;
			else
				pModuleStruct.UseCrossMesh = true;
		}

	}


	//////////////////////
	// Rotation Module 
	//////////////////////
	ImGui::Spacing();
	static bool RotationModulecheck = pModuleStruct.Module[(UINT)PARTICLE_MODULE::ROTATION];
	if (ImGui::Checkbox("##Module8", &RotationModulecheck))
	{
		if (!RotationModulecheck)
			pModuleStruct.Module[(UINT)PARTICLE_MODULE::ROTATION] = false;
		else
			pModuleStruct.Module[(UINT)PARTICLE_MODULE::ROTATION] = true;
	}
	ImGui::SameLine();
	ImGui::SeparatorText("Rotation Module");


	if (pModuleStruct.Module[(UINT)PARTICLE_MODULE::ROTATION])
	{

		// Random	: StartMin/Max와 Min/MaxSpeed 사이의 랜덤값 사용
		// Fixed	: StartMinRotation과 MinRotationSpeed를 고정값으로 사용
		ImGui::Text("RotationType");
		ImGui::SameLine(120);
		const char* szRotationType[3] = { "NONE", "Random" , "Fixed"};
		int rotationType = pModuleStruct.RotationType;
		if (ImGui::Combo("##comboRotationType", &rotationType, szRotationType, 3))
		{
			pModuleStruct.RotationType = rotationType;
		}

		if (pModuleStruct.RotationType > 0)
		{
			
			ImGui::Text("StartMinRotation");
			ImGui::SameLine(120);
			float startMinRotation[3] = { pModuleStruct.StartMinRotation.x, pModuleStruct.StartMinRotation.y, pModuleStruct.StartMinRotation.z };
			if (ImGui::DragFloat3("##StartMinRotation", startMinRotation, 0.1f, 0.0f))
			{
				// 모니터 뒤로 굴리듯이
				pModuleStruct.StartMinRotation.x = startMinRotation[0];

				// 옆으로 돌림
				pModuleStruct.StartMinRotation.y = startMinRotation[1];

				// 평면이라고 생각했을때, 시계,반시계로 돌아가는 느낌
				pModuleStruct.StartMinRotation.z = startMinRotation[2];
			}

			if (pModuleStruct.RotationType == 1)
			{
				ImGui::Text("StartMaxRotation");
				ImGui::SameLine(120);
				float startManRotation[3] = { pModuleStruct.StartMaxRotation.x, pModuleStruct.StartMaxRotation.y, pModuleStruct.StartMaxRotation.z };
				if (ImGui::DragFloat3("##StartMaxRotation", startManRotation, 0.1f, 0.0f))
				{
					pModuleStruct.StartMaxRotation.x = startManRotation[0];
					pModuleStruct.StartMaxRotation.y = startManRotation[1];
					pModuleStruct.StartMaxRotation.z = startManRotation[2];
				}
			}
	


			// 축별 회전 속도를 각도/초 단위로 지정
			ImGui::Text("MinRotationSpeed");
			ImGui::SameLine(120);
			float minRotationSpeed[3] = { pModuleStruct.MinRotationSpeed.x, pModuleStruct.MinRotationSpeed.y, pModuleStruct.MinRotationSpeed.z };
			if (ImGui::DragFloat3("##MinRotationSpeed", minRotationSpeed, 0.1f, 0.0f))
			{
				pModuleStruct.MinRotationSpeed.x = minRotationSpeed[0];
				pModuleStruct.MinRotationSpeed.y = minRotationSpeed[1];
				pModuleStruct.MinRotationSpeed.z = minRotationSpeed[2];
			}

			if (pModuleStruct.RotationType == 1)
			{
				ImGui::Text("MaxRotationSpeed");
				ImGui::SameLine(120);
				float maxRotationSpeed[3] = { pModuleStruct.MaxRotationSpeed.x, pModuleStruct.MaxRotationSpeed.y, pModuleStruct.MaxRotationSpeed.z };
				if (ImGui::DragFloat3("##MaxRotationSpeed", maxRotationSpeed, 0.1f, 0.0f))
				{
					pModuleStruct.MaxRotationSpeed.x = maxRotationSpeed[0];
					pModuleStruct.MaxRotationSpeed.y = maxRotationSpeed[1];
					pModuleStruct.MaxRotationSpeed.z = maxRotationSpeed[2];
				}
			}


			// 회전 감쇠율
			// - 0 ~ 1 값을 가지며, 1이면 즉시 정지 0.5면 매 초마다 회전 속도가 절반으로 감소하는 옵션
			ImGui::Text("RotationDamping");		
			ImGui::SameLine(120);
			float rotationDamping = pModuleStruct.RotationDamping;
			if (ImGui::DragFloat("##RotationDamping", &rotationDamping, 0.1f))
			{
				if (rotationDamping < 0)
					rotationDamping = 0.f;
				else if (rotationDamping > 1.f)
					rotationDamping = 1.f;

				pModuleStruct.RotationDamping = rotationDamping;
			}

			// 파티클이 로컬기준이다. -> 파티클의 독립적인 회전
			//			월드 기준이다 -> 여러 파티클이 같은 방향을 향할때
			ImGui::Text("RotationSpace");
			ImGui::SameLine(120);
			const char* szRotationSpace[2] = { "Local", "World" };
			int rotationSpace = pModuleStruct.RotationSpace;
			if (ImGui::Combo("##comborotationSpace", &rotationSpace, szRotationSpace, 2))
			{
				pModuleStruct.RotationSpace = rotationSpace;
			}

		}



	}


	// Rotation Module (추가)
	//UINT    RotationType;       
	//Vec3    StartMinRotation;   // 시작 최소 회전
	//Vec3    StartMaxRotation;   // 시작 최대 회전
	//Vec3    MinRotationSpeed;   // 최소 회전 속도
	//Vec3    MaxRotationSpeed;   // 최대 회전 속도
	//float   RotationDamping;    // 회전 감쇠율
	//UINT    RotationSpace;      // 0: Local, 1: World
	// 
	// 
	

}



void ParticleSystemUI::SelectParticleTex(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	// 리스트에서 더블킬릭한 항목의 이름을 받아온다.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;
		
	if (*pStr == "None")
	{
		GetTargetObject()->ParticleSystem()->SetParticleTexture(nullptr);
		return;
	}

	// 해당 항목 에셋을 찾아서, MeshRenderComponent 가 해당 메시를 참조하게 한다.

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(wstring(pStr->begin(), pStr->end()));
	if (nullptr == pTex)
		return;

	GetTargetObject()->ParticleSystem()->SetParticleTexture(pTex);
}
