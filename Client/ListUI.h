#pragma once
#include "EditorUI.h"

class ListUI :
    public EditorUI
{
private:
    vector<string>  m_vecList;
    string          m_strSelected;
    int             m_SelectedIdx;

    // DoubleClicked �� ȣ���ų Delegate
    EditorUI* m_DbclickedInst;
    EUI_DELEGATE_2  m_DbclickedFunc;

public:
    void AddItem(const string& _Item) { m_vecList.push_back(_Item); }
    void AddItem(const wstring& _Item) { m_vecList.push_back(string(_Item.begin(), _Item.end())); }

    void AddItem(const vector<string>& _vecItem) { m_vecList.insert(m_vecList.end(), _vecItem.begin(), _vecItem.end()); }
    void AddItem(const vector <wstring>& _vecItem)
    {
        for (size_t i = 0; i < _vecItem.size(); ++i)
        {
            m_vecList.push_back(string(_vecItem[i].begin(), _vecItem[i].end()));
        }
    }
    string GetSelectedString() { return m_strSelected; }
    void AddDynamicDoubleClicked(EditorUI* _Inst, EUI_DELEGATE_2 _MemFunc)
    {
        m_DbclickedInst = _Inst;
        m_DbclickedFunc = _MemFunc;
    }

public:
    virtual void Render_Update() override;

private:
    virtual void Deactivate() override
    {
        m_vecList.clear();
        m_SelectedIdx = -1;
    }
    virtual void Activate() override
    {
        m_strSelected = "";
    }

public:
    ListUI();
    ~ListUI();
};

