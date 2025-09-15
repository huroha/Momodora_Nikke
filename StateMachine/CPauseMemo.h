#pragma once
#include <Engine/CState.h>

class CPauseMemo :
    public CState
{
private:
    vector<int> m_MemoFont;

    int         m_CurIdx;
    bool        m_IsSelect;
    bool        m_OneTime;   // ��Ʈ ���� �ѹ��� -> ���� �޴��� �Ѿ�� ����.
public:
    virtual void FinalTick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CPauseMemo);
    CPauseMemo();
    ~CPauseMemo();
};

