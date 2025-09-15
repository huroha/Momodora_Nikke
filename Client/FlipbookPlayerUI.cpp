#include "pch.h"
#include "FlipbookPlayerUI.h"

#include <Engine/CFlipbookPlayer.h>
#include "Inspector.h"
#include "CImGuiMgr.h"
#include "ListUI.h"
#include <Engine/CAssetMgr.h>
#include <Engine/Ptr.h>
#include <Engine/CFlipbook.h>

FlipbookPlayerUI::FlipbookPlayerUI()
    : ComponentUI("FlipbookPlayerUI", COMPONENT_TYPE::FLIPBOOKPLAYER)
{
}

FlipbookPlayerUI::~FlipbookPlayerUI()
{
}

void FlipbookPlayerUI::Render_Update()
{

	ComponentTitle("FlipbookPlayer");

	CFlipbookPlayer* pFlipbook = GetTargetObject()->FlipbookPlayer();

	ImGui::SameLine(350);
	if (ImGui::Button("X"))
	{
		DeleteComponentTask(pFlipbook);

		Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
		pInspector->DeleteComponentCheck(GetType());
	}
	ImGui::Spacing();

	vector<Ptr<CFlipbook>> vContainer = pFlipbook->GetSpriteVector();
	ImGui::Text("Flipbook List");
	ImGui::BeginChild("##FlipbookList", ImVec2(0, 100), ImGuiChildFlags_Border);
	int count = pFlipbook->GetFlipbookCount();
	for (int i = 0; i < count; ++i)
	{
		wstring Name = vContainer[i]->GetName();
		string sName = WStringToString(Name);
		ImGui::Selectable(sName.c_str());
	}
	ImGui::EndChild();

	ImGui::Text("CurFlipbook");
	ImGui::SameLine(120);
	
	string curFName;
	wstring curName;
	if (count == 0)
		curName = L"None";
	else
	{
		curName = pFlipbook->GetCurFlipbook()->GetName();
	}
	
	curFName = WStringToString(curName);
	ImGui::Text(curFName.c_str());

	ImGui::Text("Repeat");
	ImGui::SameLine(120);
	bool Repeat = pFlipbook->GetRepeat();
	if (ImGui::Checkbox("##Repeat", &Repeat))
	{
		pFlipbook->SetRepeat(Repeat);
	}

	ImGui::Text("FPS");
	ImGui::SameLine(120);
	float FPS = pFlipbook->GetFPS();
	if (ImGui::DragFloat("##FPS", &FPS))
	{
		pFlipbook->SetFPS(FPS);
	}

	if (ImGui::Button("Add Flipbook"))
	{
		// ListUI �� Ȱ��ȭ ��Ű��
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->SetName("Add Filpbook");
		pListUI->SetActive(true);

		// ListUI �� �־��� ���ڿ� ���� ��������
		pListUI->AddItem("None");

		map<wstring, Ptr<CAsset>>  mapFlipbookAsset = CAssetMgr::GetInst()->GetAssets(ASSET_TYPE::FLIPBOOK);
		//CAssetMgr::GetStateInfo(vecStateNames);
		map<wstring, Ptr<CAsset>>::iterator iter = mapFlipbookAsset.begin();
		for (; iter != mapFlipbookAsset.end(); ++iter)
		{
			pListUI->AddItem(iter->first);
		}
		// ���� Ŭ�� �� ȣ���ų �Լ� ���
		pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&FlipbookPlayerUI::AddFlipbook_List);
	}

	if (ImGui::Button("Remove Last Flipbook"))
	{
		pFlipbook->RemoveFlipbook();
	}



}

void FlipbookPlayerUI::AddFlipbook_List(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{

	// ����Ʈ���� ����ų���� �׸��� �̸��� �޾ƿ´�.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;

	if (*pStr == "None")
		return;

	// �ش� Flipbook�� ã�Ƽ�, FlipbookPlayer �� �ش� Flipbook�� �߰��ϰ� �Ѵ�.

	CFlipbookPlayer* pFlipbookPlayer= GetTargetObject()->FlipbookPlayer();

	wstring FlipbookName = wstring(pStr->begin(), pStr->end());

	pFlipbookPlayer->AddFlipbook(pFlipbookPlayer->GetFlipbookCount(), CAssetMgr::GetInst()->Load<CFlipbook>(FlipbookName, FlipbookName));
	pFlipbookPlayer->Play(0, 8, false);
}

