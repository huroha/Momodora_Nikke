#include "pch.h"
#include "CPauseMemo.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CFontMgr.h>

#include <Scripts/CPauseFont.h>

CPauseMemo::CPauseMemo()
    : m_CurIdx(0)
    , m_IsSelect(false)
    , m_OneTime(false)
{

}

CPauseMemo::~CPauseMemo()
{
}


void CPauseMemo::FinalTick()
{
    if (!CLevelMgr::GetInst()->IsPauseState())
        return;

    // 선택하고 있지않을때에만
    if (!m_IsSelect)
    {
        // 메인으로 넘어감
        if (KEY_TAP(KEY::RIGHT))
            GetStateMachine()->ChangeState(L"CPauseMain");
        // 옵션으로 넘어감
        else if (KEY_TAP(KEY::LEFT))
            GetStateMachine()->ChangeState(L"CPauseOption");
    }
   

    if (KEY_TAP(KEY::SPACE))
    {
        m_IsSelect = !m_IsSelect;
        m_OneTime = false;
    }


    // 여기서 보여지는 메모 관리해야할듯.
    if (m_IsSelect)
    {
        if (KEY_TAP(KEY::DOWN))
        {

            ++m_CurIdx;
            if (m_CurIdx >= m_MemoFont.size() / 3)
                m_CurIdx = m_MemoFont.size() / 3 -1;
            CFontMgr::GetInst()->UpdateColor(m_MemoFont[(m_CurIdx-1) * 3], FONT_RGBA(100, 100, 100, 255), true);
            CFontMgr::GetInst()->SetPauseActive(m_MemoFont[(m_CurIdx - 1) * 3 + 1], false);
            CFontMgr::GetInst()->SetPauseActive(m_MemoFont[(m_CurIdx - 1) * 3 + 2], false);
        }
        else if (KEY_TAP(KEY::UP))
        {
            --m_CurIdx;
            if (m_CurIdx < 0)
                m_CurIdx = 0;

            CFontMgr::GetInst()->UpdateColor(m_MemoFont[(m_CurIdx+1) * 3], FONT_RGBA(100, 100, 100, 255), true);
            CFontMgr::GetInst()->SetPauseActive(m_MemoFont[(m_CurIdx + 1) * 3 + 1], false);
            CFontMgr::GetInst()->SetPauseActive(m_MemoFont[(m_CurIdx + 1) * 3 + 2], false);

        }

        CFontMgr::GetInst()->UpdateColor(m_MemoFont[m_CurIdx*3], FONT_RGBA(255, 255, 255, 255),true);
        CFontMgr::GetInst()->SetPauseActive(m_MemoFont[m_CurIdx * 3 + 1],true);
        CFontMgr::GetInst()->SetPauseActive(m_MemoFont[m_CurIdx * 3 + 2], true);

    }
    else
    {
        if (!m_OneTime)
        {
            CFontMgr::GetInst()->UpdateColor(m_MemoFont[m_CurIdx * 3], FONT_RGBA(100, 100, 100, 255), true);
            CFontMgr::GetInst()->SetPauseActive(m_MemoFont[m_CurIdx * 3 + 1], false);
            CFontMgr::GetInst()->SetPauseActive(m_MemoFont[m_CurIdx * 3 + 2], false);
            m_OneTime = true;
            m_CurIdx = 0;
        }

    }

}

void CPauseMemo::Enter()
{
    
    CGameObject* pObj = CLevelMgr::GetInst()->FindObjectByName(L"BG_Pasue");

    CPauseFont* pBgTarget = (CPauseFont*)pObj->GetScripts()[0];
    m_MemoFont = pBgTarget->GetMemoFont();
    pBgTarget->ResetActiveCheck();
    
    pObj = CLevelMgr::GetInst()->FindObjectByName(L"MemoList");
    pObj->GetRenderComponent()->SetActive(true);

    // 언제나 메모는 초기화 되어서 보여줌
    for (int i = 0; i < m_MemoFont.size()/3; ++i)
    {
        CFontMgr::GetInst()->SetPauseActive(m_MemoFont[i * 3], true);
    }
    pObj = CLevelMgr::GetInst()->FindObjectByName(L"Point_Pause");
    pObj->Transform()->SetRelativePos(Vec3(-472.f, 316.f, 100.f));

    // 선택 초기화
    m_IsSelect = false;
    m_CurIdx = 0;
}

void CPauseMemo::Exit()
{

    CGameObject* pObj = CLevelMgr::GetInst()->FindObjectByName(L"MemoList");
    pObj->GetRenderComponent()->SetActive(false);
    
    for (int i = 0; i < m_MemoFont.size(); ++i)
    {
        CFontMgr::GetInst()->SetPauseActive(m_MemoFont[i], false);
    }
    m_MemoFont.clear();
}
