#include "pch.h"
#include "MaterialUI.h"

#include <Engine/CPathMgr.h>
#include <Engine/CAssetMgr.h>

#include "CImGuiMgr.h"
#include "ListUI.h"
#include "TreeUI.h"
#include "ParamUI.h"


MaterialUI::MaterialUI()
	: AssetUI("Material", ASSET_TYPE::MATERIAL)
{
}

MaterialUI::~MaterialUI()
{
}

void MaterialUI::Render_Update()
{
	AssetTitle();

	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
	Ptr<CGraphicShader> pShader = pMtrl->GetShader();

	ImGui::Text("Name");
	ImGui::SameLine(100);
	string name = string(pMtrl.Get()->GetName().begin(), pMtrl.Get()->GetName().end());

	static char buf[255];
	snprintf(buf, sizeof(buf), "%s", name.c_str());
	if (ImGui::InputText("##UTF-8 input", buf, IM_ARRAYSIZE(buf)))
	{

		int wlen = MultiByteToWideChar(CP_UTF8, 0, buf, -1, NULL, 0);
		wstring Name(wlen - 1, 0); // -1�� null terminator�� ����
		MultiByteToWideChar(CP_UTF8, 0, buf, -1, &Name[0], wlen);
		pMtrl->SetName(Name);

		CAssetMgr::GetInst()->AssetReFresh();
	}

	string ShaderName;

	if (nullptr == pShader)
		ShaderName = "None";
	else
	{
		ShaderName = string(pShader->GetKey().begin(), pShader->GetKey().end());
	}

	ImGui::Text("Shader");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150);
	ImGui::InputText("##MeshName", (char*)ShaderName.c_str(), ShaderName.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		if (ImGui::AcceptDragDropPayload("ContentUI"))
		{
			const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();
			TreeNode* pNode = *((TreeNode**)pPayload->Data);
			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (pAsset->GetAssetType() == ASSET_TYPE::GRAPHIC_SHADER)
			{
				pMtrl->SetShader((CGraphicShader*)pAsset.Get());
				ImGui::SetWindowFocus(nullptr);
			}
		}

		ImGui::EndDragDropTarget();
	}


	ImGui::SameLine();
	if (ImGui::Button("##ShaderBtn", ImVec2(18.f, 18.f)))
	{
		// ListUI �� Ȱ��ȭ ��Ű��
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->SetName("GraphicShader");
		pListUI->SetActive(true);

		// ListUI �� �־��� ���ڿ� ���� ��������
		pListUI->AddItem("None");

		vector<wstring> vecAssetNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::GRAPHIC_SHADER, vecAssetNames);
		pListUI->AddItem(vecAssetNames);

		// ���� Ŭ�� �� ȣ���ų �Լ� ���
		pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&MaterialUI::SelectGraphicShader);
	}

	// Shader ���� ��û�ϴ� �Ķ���� ������ ������ش�.
	ShaderParameter();


	// ������ ���Ϸ� �����ϱ�
	if (ImGui::Button("SAVE"))
	{
		Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
		assert(pMtrl.Get());

		wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + pMtrl->GetKey();
		pMtrl->Save(strFilePath);
	}



}



void MaterialUI::ShaderParameter()
{
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
	Ptr<CGraphicShader> pShader = pMtrl->GetShader();

	if (nullptr == pShader)
		return;

	const vector<tScalarParam>& vecScalar = pShader->GetScalarParam();

	for (size_t i = 0; i < vecScalar.size(); ++i)
	{
		switch (vecScalar[i].eParam)
		{
		case INT_0:
		case INT_1:
		case INT_2:
		case INT_3:
		{
			int* pData = (int*)pMtrl->GetScalarParam(vecScalar[i].eParam);
			int Data = *pData;

			if (ParamUI::Param_Int(vecScalar[i].Desc, &Data, vecScalar[i].Drag))
			{
				pMtrl->SetScalarParam(vecScalar[i].eParam, Data);
			}
		}
		break;
		case FLOAT_0:
		case FLOAT_1:
		case FLOAT_2:
		case FLOAT_3:
		{
			float* pData = (float*)pMtrl->GetScalarParam(vecScalar[i].eParam);
			float Data = *pData;

			if (ParamUI::Param_Float(vecScalar[i].Desc, &Data, vecScalar[i].Drag))
			{
				pMtrl->SetScalarParam(vecScalar[i].eParam, Data);
			}
		}
		break;
		case VEC2_0:
		case VEC2_1:
		case VEC2_2:
		case VEC2_3:
		{
			Vec2* pData = (Vec2*)pMtrl->GetScalarParam(vecScalar[i].eParam);
			Vec2 Data = *pData;

			if (ParamUI::Param_Vec2(vecScalar[i].Desc, &Data, vecScalar[i].Drag))
			{
				pMtrl->SetScalarParam(vecScalar[i].eParam, Data);
			}
		}
		break;
		case VEC4_0:
		case VEC4_1:
		case VEC4_2:
		case VEC4_3:
		{
			Vec4* pData = (Vec4*)pMtrl->GetScalarParam(vecScalar[i].eParam);
			Vec4 Data = *pData;

			if (ParamUI::Param_Vec4(vecScalar[i].Desc, &Data, vecScalar[i].Drag))
			{
				pMtrl->SetScalarParam(vecScalar[i].eParam, Data);
			}
		}
		break;
		case MAT_0:
		case MAT_1:

			break;
		}
	}

	const vector<tTexParam>& vecTex = pShader->GetTexParam();
	for (size_t i = 0; i < vecTex.size(); ++i)
	{
		Ptr<CTexture> pTex = pMtrl->GetTexParam(vecTex[i].eParam);

		if (ParamUI::Param_Tex(vecTex[i].Desc, pTex
			, this, (EUI_DELEGATE_2)&MaterialUI::SelectTexture))
		{
			m_OpenTexType = vecTex[i].eParam;
			pMtrl->SetTexParam(vecTex[i].eParam, pTex);
		}
	}

}

void MaterialUI::SelectGraphicShader(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	// ����Ʈ���� ����ų���� �׸��� �̸��� �޾ƿ´�.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;

	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
	if (nullptr == pMtrl)
		return;

	if (*pStr == "None")
	{
		pMtrl->SetShader(nullptr);
		return;
	}

	// �ش� �׸� ������ ã�Ƽ�, MeshRenderComponent �� �ش� �޽ø� �����ϰ� �Ѵ�.
	Ptr<CGraphicShader> pShader = CAssetMgr::GetInst()->FindAsset<CGraphicShader>(wstring(pStr->begin(), pStr->end()));
	if (nullptr == pShader)
		return;

	pMtrl->SetShader(pShader);
}

void MaterialUI::SelectTexture(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	// ����Ʈ���� ����ų���� �׸��� �̸��� �޾ƿ´�.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();

	if (*pStr == "None")
	{
		pMtrl->SetTexParam(m_OpenTexType, nullptr);
		return;
	}

	// �ش� �׸� ������ ã�Ƽ�, MeshRenderComponent �� �ش� �޽ø� �����ϰ� �Ѵ�.
	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(wstring(pStr->begin(), pStr->end()));
	if (nullptr == pTex)
		return;

	pMtrl->SetTexParam(m_OpenTexType, pTex);
}