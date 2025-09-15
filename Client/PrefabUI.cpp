#include "pch.h"
#include "PrefabUI.h"

#include <Engine/CPrefab.h>
#include <Engine/CPathMgr.h>

#include "ListUI.h"
#include "CImGuiMgr.h"

PrefabUI::PrefabUI()
	: AssetUI("Prefab", ASSET_TYPE::PREFAB)
{
}

PrefabUI::~PrefabUI()
{
}

void PrefabUI::Render_Update()
{
	AssetTitle();

	Ptr<CPrefab> pAsset = dynamic_cast<CPrefab*>(GetAsset().Get());
	assert(pAsset.Get());

	ImGui::Text("Name");
	ImGui::SameLine(100);

	string strKey = string(pAsset->GetKey().begin(), pAsset->GetKey().end());
	ImGui::InputText("##PrefabName", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_ReadOnly);
	//int layerIdx = pNewObject->GetLayerIdx();

	if (ImGui::Button("Instantiate"))
	{
		// ListUI �� Ȱ��ȭ ��Ű��
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->SetName("LayerIdx");
		pListUI->SetActive(true);


		// ListUI �� �־��� ���ڿ� ���� ��������
		pListUI->AddItem("None");

		vector<wstring> vecStateNames;
		for (int i = 0; i < MAX_LAYER; ++i)
		{
			pListUI->AddItem(layerNames[i]);
		}

		// ���� Ŭ�� �� ȣ���ų �Լ� ���
		pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&PrefabUI::SelectLayerIdx);



	}

	if (ImGui::Button("Save"))
	{
		wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

		// ���� ��� ���ڿ�
		wchar_t szFilePath[255] = {};

		OPENFILENAME Desc = {};

		Desc.lStructSize = sizeof(OPENFILENAME);
		Desc.hwndOwner = nullptr;
		Desc.lpstrFile = szFilePath;
		Desc.nMaxFile = 255;
		Desc.lpstrFilter = L"Prefab\0*.pref\0ALL\0*.*";
		Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		Desc.lpstrInitialDir = strContentPath.c_str();

		if (GetSaveFileName(&Desc))
		{
			if (wcslen(szFilePath) != 0)
			{
				pAsset->Save(szFilePath);
			}


		}
	}
}


void PrefabUI::SelectLayerIdx(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	Ptr<CPrefab> pAsset = dynamic_cast<CPrefab*>(GetAsset().Get());

	// ����Ʈ���� ����ų���� �׸��� �̸��� �޾ƿ´�.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;

	if (*pStr == "None")
		return;

	// �ش� Layer�̸� �� ã�Ƽ�, layerIdx�� ��ȯ�ؼ� Prefab ����

	wstring LayerName = wstring(pStr->begin(), pStr->end());
	string sName = WStringToString(LayerName);
	int LayerIdx = 0;
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (sName == layerNames[i])
			LayerIdx = i;
	}
    CGameObject* pNewObject = pAsset->Instantiate();
    CreateObject(pNewObject, LayerIdx, false);
}
