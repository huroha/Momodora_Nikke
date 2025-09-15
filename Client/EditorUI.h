#pragma once

#include "imgui/imgui.h"

class EditorUI
{
private:
	string				m_Name;
	const string		m_ID;

	EditorUI* m_ParentUI;
	vector<EditorUI*>	m_vecChildUI;

	ImVec2				m_ChildSize;
	bool				m_Active;
	bool				m_Modal;

public:
	void SetName(const string& _name) { m_Name = _name; }
	const string& GetName() { return m_Name; }
	const string& GetID() { return m_ID; }

	EditorUI* GetParentUI() { return m_ParentUI; }
	const vector<EditorUI*>& GetChildUI() { return m_vecChildUI; }

	ImVec2 GetChildSize() { return m_ChildSize; }
	void SetChildSize(ImVec2 _Size) { m_ChildSize = _Size; }

	EditorUI* AddChildUI(EditorUI* _Child) { m_vecChildUI.push_back(_Child); _Child->m_ParentUI = this; return _Child; }

public:
	void SetActive(bool _bActive);
	bool IsActive() { return m_Active; }

	void SetModal(bool _Modal) { m_Modal = _Modal; }
	bool IsModal() { return m_Modal; }


public:
	virtual void Tick();
	virtual void Render();
	virtual void Render_Update() = 0;

private:
	void Render_Modal();
	void Render_Modalless();
	void Render_Child();


	virtual void Activate() {}
	virtual void Deactivate() {};

public:
	EditorUI(const string& _ID);
	virtual ~EditorUI();
};

// �ɹ��Լ� ������Ÿ�� ������(EidtorUI �� �ɹ��Լ�, ���ڴ� void, ��ȯŸ�� void)
typedef void(EditorUI::* EUI_DELEGATE)(void);
typedef void(EditorUI::* EUI_DELEGATE_1)(DWORD_PTR);
typedef void(EditorUI::* EUI_DELEGATE_2)(DWORD_PTR, DWORD_PTR);