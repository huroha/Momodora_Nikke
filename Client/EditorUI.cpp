#include "pch.h"
#include "EditorUI.h"

EditorUI::EditorUI(const string& _ID)
	: m_Active(true)
	, m_ID(_ID)
	, m_ParentUI(nullptr)
	, m_Modal(false)
{
}

EditorUI::~EditorUI()
{
	DeleteVec(m_vecChildUI);
}



void EditorUI::Tick()
{
	if (!m_Active)
		return;

}

void EditorUI::Render()
{
	if (!m_Active)
		return;

	// �ֻ��� �θ� UI
	if (nullptr == m_ParentUI)
	{
		// Modalless
		if (false == m_Modal)
		{
			Render_Modalless();
		}

		// Modal
		else
		{
			Render_Modal();
		}
	}

	// Child UI �� ���
	else
	{
		Render_Child();
	}
}

void EditorUI::Render_Modalless()
{
	string strID = m_Name + m_ID;

	bool Active = m_Active;

	ImGui::Begin(strID.c_str(), &Active);

	// ImGui �� X ��ư�� ���� ���
	if (Active != m_Active)
		SetActive(Active);

	Render_Update();

	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		if (!m_vecChildUI[i]->IsActive())
		{
			continue;
		}

		m_vecChildUI[i]->Render();

		for (int j = i + 1; j < m_vecChildUI.size(); ++j)
		{
			if (m_vecChildUI[j]->IsActive())
			{
				ImGui::Separator();
				break;
			}
		}
	}


	ImGui::End();
}


void EditorUI::Render_Modal()
{
	string strID = m_Name + m_ID;

	bool Active = m_Active;

	ImGui::OpenPopup(strID.c_str());
	if (ImGui::BeginPopupModal(strID.c_str(), &Active))
	{
		Render_Update();

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			if (!m_vecChildUI[i]->IsActive())
			{
				continue;
			}

			m_vecChildUI[i]->Render();

			for (int j = i + 1; j < m_vecChildUI.size(); ++j)
			{
				if (m_vecChildUI[j]->IsActive())
				{
					ImGui::Separator();
					break;
				}
			}
		}

		ImGui::EndPopup();
	}
	else
	{
		SetActive(Active);
	}
}

void EditorUI::Render_Child()
{
	string strID = m_Name + m_ID;

	ImGui::BeginChild(strID.c_str(), m_ChildSize);

	Render_Update();

	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->Render();

		for (int j = i + 1; j < m_vecChildUI.size(); ++j)
		{
			if (m_vecChildUI[j]->IsActive())
			{
				ImGui::Separator();
				break;
			}
		}
	}

	ImGui::EndChild();
}


void EditorUI::SetActive(bool _bActive)
{
	if (m_Active == _bActive)
		return;

	m_Active = _bActive;

	if (m_Active)
		Activate();
	else
		Deactivate();
}