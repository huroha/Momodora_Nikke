#include "pch.h"
#include "StateMachineUI.h"

#include <Engine/CStateMachine.h>
#include "CImGuiMgr.h"
#include "Inspector.h"
#include "ListUI.h"
#include <StateMachine/CStateMgr.h>



StateMachineUI::StateMachineUI()
	: ComponentUI("StateMachineUI", COMPONENT_TYPE::STATEMACINE)
    , m_CurStateIdx(0)
{
    m_StringStorage.clear();
    m_StatePointers.clear();
    m_StringStorage.push_back("None");
    m_StatePointers.push_back(m_StringStorage[0].c_str());
}

StateMachineUI::~StateMachineUI()
{
}

void StateMachineUI::Render_Update()
{

	ComponentTitle("StateMachineUI");

	CStateMachine* pStateMachine = GetTargetObject()->StateMachine();

    map<wstring, CState*> mapState = pStateMachine->GetStateContainer();

    ImGui::SameLine(350);
    if (ImGui::Button("X"))
    {
        DeleteComponentTask(pStateMachine);

        Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
        pInspector->DeleteComponentCheck(GetType());
    }

    if (ImGui::Button("AddState"))
    {
        // ListUI �� Ȱ��ȭ ��Ű��
        ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
        pListUI->SetName("Add State");
        pListUI->SetActive(true);

        // ListUI �� �־��� ���ڿ� ���� ��������
        pListUI->AddItem("None");

        vector<wstring> vecStateNames;
        CStateMgr::GetStateInfo(vecStateNames);
        pListUI->AddItem(vecStateNames);

        // ���� Ŭ�� �� ȣ���ų �Լ� ���
        pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&StateMachineUI::AddState_List);
    }
    ImGui::SameLine(120);
    if (ImGui::Button("RemoveState"))
    {
        // ListUI �� Ȱ��ȭ ��Ű��
        ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
        pListUI->SetName("Remove State");
        pListUI->SetActive(true);

        // ListUI �� �־��� ���ڿ� ���� ��������
        pListUI->AddItem("None");

        vector<wstring> vecStateNames;
        map<wstring, CState*>::iterator iter = mapState.begin();
        for (; iter != mapState.end(); ++iter)
        {
            pListUI->AddItem(iter->first);
        }
        // ���� Ŭ�� �� ȣ���ų �Լ� ���
        pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&StateMachineUI::RemoveState_List);
    }


	ImGui::Text("CurState");
	ImGui::SameLine(120);

	wstring CurStateName = pStateMachine->GetCurStateName();
	string sName = WStringToString(CurStateName);
	ImGui::Text(sName.c_str());


	ImGui::Text("ChangeState");
	ImGui::SameLine(120);


 

    // ���� ������ Ŭ�����ϰ� "None" ���� �߰�
    m_StringStorage.clear();
    m_StatePointers.clear();
    m_StringStorage.push_back("None");
    m_StatePointers.push_back(m_StringStorage[0].c_str());

    // ������ ���µ� �߰�
    for (const auto& pair : mapState)
    {
        m_StringStorage.push_back(WStringToString(pair.first));
        m_StatePointers.push_back(m_StringStorage.back().c_str());
    }

    if (ImGui::Combo("##StateCombo", &m_CurStateIdx,
        m_StatePointers.data(), (int)m_StatePointers.size()))
    {
        if (m_CurStateIdx == 0)  // None ����
        {
            pStateMachine->ChangeStateNone();
        }
        else
        {
            wstring selectedState = StringToWString(m_StringStorage[m_CurStateIdx]);
            pStateMachine->ChangeState(selectedState);
        }
    }



}


void StateMachineUI::AddState_List(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{

    // ����Ʈ���� ����ų���� �׸��� �̸��� �޾ƿ´�.
    ListUI* pListUI = (ListUI*)_ListUI;
    string* pStr = (string*)_SelectString;

    if (*pStr == "None")
        return;

    // �ش� state�� ã�Ƽ�, StateMachine �� �ش� State�� �߰��ϰ� �Ѵ�.

    CStateMachine* pStateMachine = GetTargetObject()->StateMachine();
    wstring StateName = wstring(pStr->begin(), pStr->end());
    if (nullptr != pStateMachine->FindState(StateName))
        return;
    else
    {
        pStateMachine->AddState(StateName, CStateMgr::GetState(StateName));
    }

}

void StateMachineUI::RemoveState_List(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{

    // ����Ʈ���� ����ų���� �׸��� �̸��� �޾ƿ´�.
    ListUI* pListUI = (ListUI*)_ListUI;
    string* pStr = (string*)_SelectString;

    if (*pStr == "None")
        return;

    // �ش� state�� ã�Ƽ�, StateMachine �� �ش� State�� �����ϰ� �Ѵ�.

    CStateMachine* pStateMachine = GetTargetObject()->StateMachine();
    wstring StateName = wstring(pStr->begin(), pStr->end());
    if (nullptr != pStateMachine->FindState(StateName))
    {
        pStateMachine->RemoveState(StateName);
    }
}
