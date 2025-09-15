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
		// ListUI 를 활성화 시키기
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->SetName("LayerIdx");
		pListUI->SetActive(true);


		// ListUI 에 넣어줄 문자열 정보 가져오기
		pListUI->AddItem("None");

		vector<wstring> vecStateNames;
		for (int i = 0; i < MAX_LAYER; ++i)
		{
			pListUI->AddItem(layerNames[i]);
		}

		// 더블 클릭 시 호출시킬 함수 등록
		pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&PrefabUI::SelectLayerIdx);



	}

	if (ImGui::Button("Save"))
	{
		wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

		// 파일 경로 문자열
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

	// 리스트에서 더블킬릭한 항목의 이름을 받아온다.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;

	if (*pStr == "None")
		return;

	// 해당 Layer이름 을 찾아서, layerIdx를 반환해서 Prefab 생성

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
