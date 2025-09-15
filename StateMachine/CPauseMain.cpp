#include "pch.h"
#include "CPauseMain.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Scripts/CPauseFont.h>

CPauseMain::CPauseMain()
{
}

CPauseMain::~CPauseMain()
{
}

void CPauseMain::FinalTick()
{
    if (CLevelMgr::GetInst()->IsPauseState())
    {
        // 옵션으로 넘어감
        if (KEY_TAP(KEY::RIGHT))
        {
            GetStateMachine()->ChangeState(L"CPauseOption");
        }
        // 조작으로 넘어감
        else if (KEY_TAP(KEY::LEFT))
        {
            GetStateMachine()->ChangeState(L"CPauseMemo");
        }
    }

}

void CPauseMain::Enter()
{
    CGameObject* pObj;
    pObj = CLevelMgr::GetInst()->FindObjectByName(L"Character_Portrait");
    m_vecObj.push_back(pObj);
    pObj = CLevelMgr::GetInst()->FindObjectByName(L"Point_Pause");
    m_vecObj.push_back(pObj);
    pObj = CLevelMgr::GetInst()->FindObjectByName(L"Point_Pause2");
    m_vecObj.push_back(pObj);
    pObj = CLevelMgr::GetInst()->FindObjectByName(L"MemoList");
    pObj->GetRenderComponent()->SetActive(false);

    m_vecObj[0]->GetRenderComponent()->SetActive(true);
    m_vecObj[0]->GetChild()[0]->GetRenderComponent()->SetActive(true);
    m_vecObj[0]->GetChild()[1]->GetRenderComponent()->SetActive(true);


    m_vecObj[1]->Transform()->SetRelativePos(Vec3(4.f, 316.f, 100.f));
    m_vecObj[2]->GetRenderComponent()->SetActive(false);

    pObj = CLevelMgr::GetInst()->FindObjectByName(L"BG_Pasue");
    CPauseFont* pBgTarget = (CPauseFont*)pObj->GetScripts()[0];
    pBgTarget->ResetActiveCheck();

}

void CPauseMain::Exit()
{
    // 초상화 관련해서 다 끔.
    m_vecObj[0]->GetRenderComponent()->SetActive(false);
    m_vecObj[0]->GetChild()[0]->GetRenderComponent()->SetActive(false);
    m_vecObj[0]->GetChild()[1]->GetRenderComponent()->SetActive(false);


    m_vecObj.clear();
}