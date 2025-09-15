#include "pch.h"
#include "TransformUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include "CImGuiMgr.h"
#include "Outliner.h"

TransformUI::TransformUI()
	: ComponentUI("TransformUI", COMPONENT_TYPE::TRANSFORM)
{

}

TransformUI::~TransformUI()
{

}

void TransformUI::Render_Update()
{
	ComponentTitle("Transform");

	CTransform* pTransform = GetTargetObject()->Transform();

	Vec3 vPos = pTransform->GetRelativePos();
	Vec3 vScale = pTransform->GetRelativeScale();
	Vec3 vRot = pTransform->GetRelativeRotation();

	ImGui::Text("Name");
	ImGui::SameLine(100);
	string name = string(GetTargetObject()->GetName().begin(),GetTargetObject()->GetName().end());

	static char buf[255];
	snprintf(buf, sizeof(buf), "%s", name.c_str());
	if (ImGui::InputText("##UTF-8 input", buf, IM_ARRAYSIZE(buf)))
	{

		int wlen = MultiByteToWideChar(CP_UTF8, 0, buf, -1, NULL, 0);
		wstring Name(wlen - 1, 0); // -1은 null terminator를 제외
		MultiByteToWideChar(CP_UTF8, 0, buf, -1, &Name[0], wlen);
		GetTargetObject()->SetName(Name);
		
		Outliner* pOutliner = (Outliner*)CImGuiMgr::GetInst()->FindUI("Outliner");
		pOutliner->RenewGameObject();

	}

	ImGui::Text("LayerIdx");
	ImGui::SameLine(100);
	int LayerIdx = GetTargetObject()->GetLayerIdx();
	ImGui::Text("%d", LayerIdx);

	ImGui::SameLine(120);
	ImGui::Text(layerNames[LayerIdx]);

	ImGui::Text("Position");
	ImGui::SameLine(100);
	if (ImGui::DragFloat3("##Pos", vPos))
	{
		pTransform->SetRelativePos(vPos);
	}

	ImGui::Text("Scale");
	ImGui::SameLine(100);
	if (ImGui::DragFloat3("##Scale", vScale))
	{
		pTransform->SetRelativeScale(vScale);
	}

	ImGui::Text("Rotation");
	ImGui::SameLine(100);

	vRot = (vRot / XM_PI) * 180.f;

	if (ImGui::DragFloat3("##Rotation", vRot, 0.1f))
	{
		vRot = (vRot / 180) * XM_PI;
		pTransform->SetRelativeRotation(vRot);
	}

	ImGui::Text("Independent Scale");
	ImGui::SameLine(140);
	bool IndependentScale = pTransform->GetIndependentScale();
	if (ImGui::Checkbox("##Module1", &IndependentScale))
	{
		pTransform->SetIndependentScale(IndependentScale);
	}

}