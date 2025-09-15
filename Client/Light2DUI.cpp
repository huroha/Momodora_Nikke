#include "pch.h"
#include "Light2DUI.h"

#include <Engine/CLight2D.h>
#include "CImGuiMgr.h"
#include "Inspector.h"

Light2DUI::Light2DUI()
	: ComponentUI("Light2DUI", COMPONENT_TYPE::LIGHT2D)
{
}

Light2DUI::~Light2DUI()
{

}

void Light2DUI::Render_Update()
{
	CLight2D* pLight2D = GetTargetObject()->Light2D();

	ComponentTitle("Light2D");

	ImGui::SameLine(350);
	if (ImGui::Button("X"))
	{
		DeleteComponentTask(pLight2D);

		Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
		pInspector->DeleteComponentCheck(GetType());
	}

	tLight2DInfo LightInfo = pLight2D->GetLight2DInfo();

	//Light_Type
	ImGui::Text("Light Type");
	ImGui::SameLine(100);
	const char* szLightType[5] = { "Directional", "Point","Point_Advance","Spot_Advanced","Spot"};
	int LightType = pLight2D->GetLight2DInfo().Type;
	ImGui::Combo("##combo", &LightType, szLightType, 5);
	pLight2D->SetLightType((LIGHT_TYPE)LightType);


	//Radius
	ImGui::Text("Radius");
	ImGui::SameLine(100);
	if (ImGui::DragFloat("##Light2D Radius", &LightInfo.Radius))
	{
		pLight2D->SetRadius(LightInfo.Radius);
	}
	//Radius
	ImGui::Text("InnerRadius");
	ImGui::SameLine(100);
	if (ImGui::DragFloat("##Light2D Inner Radius", &LightInfo.InnerRadius))
	{
		pLight2D->SetInnerRadius(LightInfo.InnerRadius);
	}

	//Angle
	ImGui::Text("Angle");
	ImGui::SameLine(100);
	if (ImGui::DragFloat("##Light2D Angle", &LightInfo.Angle))
	{
		pLight2D->SetAngle(LightInfo.Angle);
	}

	//Direction
	ImGui::Text("Direction");
	ImGui::SameLine(100);
	static float LightDir[3] = { LightInfo.vDir.x, LightInfo.vDir.y, LightInfo.vDir.z };
	if (ImGui::DragFloat3("##Light2D Direction", LightDir))
	{
		LightInfo.vDir.x = LightDir[0];
		LightInfo.vDir.y = LightDir[1];
		LightInfo.vDir.z = LightDir[2];

		pLight2D->SetLightDir(Vec3(LightInfo.vDir.x, LightInfo.vDir.y, LightInfo.vDir.z));
	}


	//Color
	ImGui::Text("Color");
	ImGui::SameLine(100);
	static Vec3 color = LightInfo.vColor;
	static bool alpha_preview = true;
	static bool alpha_half_preview = false;
	static bool drag_and_drop = true;
	static bool options_menu = true;
	static bool hdr = false;

	ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
	if (ImGui::ColorEdit3("##MyColor", (float*)&color, misc_flags))
	{
		pLight2D->SetLightColor(color);
	}

}