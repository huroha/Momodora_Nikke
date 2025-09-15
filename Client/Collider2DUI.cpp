#include "pch.h"
#include "Collider2DUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CCollider2D.h>
#include <Engine/func.h>
#include "Inspector.h"
#include "CImGuiMgr.h"

Collider2DUI::Collider2DUI()
	: ComponentUI("Collider2DUI", COMPONENT_TYPE::COLLIDER2D)
{
}

Collider2DUI::~Collider2DUI()
{
}

void Collider2DUI::Render_Update()
{
	ComponentTitle("Collider2D");

	CCollider2D* pCollider2D = GetTargetObject()->Collider2D();

	ImGui::SameLine(350);
	if (ImGui::Button("X"))
	{
		DeleteComponentTask(pCollider2D);

		Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
		pInspector->DeleteComponentCheck(GetType());
	}

	Vec2 vOffset = pCollider2D->GetOffset();
	Vec2 vScale = pCollider2D->GetScale();
	bool bIndependent = pCollider2D->IsIndependentScale();

	ImGui::Text("Scale");
	ImGui::SameLine(100);
	if (ImGui::DragFloat2("##ColliderScale", vScale))
	{
		pCollider2D->SetScale(vScale);
	}

	ImGui::Text("Offset");
	ImGui::SameLine(100);
	if (ImGui::DragFloat2("##ColliderOffset", vOffset))
	{
		pCollider2D->SetOffset(vOffset);
	}

	ImGui::Text("Independent");
	ImGui::SameLine(100);
	if (ImGui::Checkbox("##Independent", &bIndependent))
	{
		pCollider2D->SetIndependentScale(bIndependent);
	}


	ImGui::Text("Active Check");
	ImGui::SameLine(100);
	const char* szActive[3] = { "ACTIVE","SEMIACTIVE" ,"DEACTIVE"};
	int enumActive = (int)pCollider2D->GetState();
	if (ImGui::Combo("##comboBlockSpawnShape", &enumActive, szActive, 3))
	{
		if (enumActive == 0)
			pCollider2D->Activate();
		else
		{
			pCollider2D->Deactivate();


		}
	}


}