#include "pch.h"
#include "CNpcTemple.h"

#include <Engine/CFontMgr.h>
#include <Engine/CLevelMgr.h>

CNpcTemple::CNpcTemple()
    : CScript((UINT)SCRIPT_TYPE::NPCTEMPLE)
    , m_DialogStart(0)
    , m_Start(false)
    , m_Next(false)
    , m_MainCamera(nullptr)
    , m_DialogBox(nullptr)
{
}

CNpcTemple::~CNpcTemple()
{
}

void CNpcTemple::SaveComponent(FILE* _File)
{
}

void CNpcTemple::LoadComponent(FILE* _File)
{
}

void CNpcTemple::Begin()
{
    FlipbookPlayer()->Play(0, 9, true);

    wstring Start = L"´ëÈ­";

    m_DialogStart = CFontMgr::GetInst()->RegisterText(Start, 928.f, 544.f, 20.f, FONT_RGBA(255, 255, 255, 255));

    CFontMgr::GetInst()->SetActive(m_DialogStart, false);
    m_Child = GetOwner()->GetChild();

    m_MainCamera = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera");
    m_DialogBox = CLevelMgr::GetInst()->FindObjectByName(L"Dialog_Box1");
    m_DialogBox->GetRenderComponent()->SetActive(false);
    // ÀÚ½Ä ¸»Ç³¼±µé ¼û±èÃ³¸®
    for (int i = 0; i < m_Child.size(); ++i)
    {
        m_Child[i]->GetRenderComponent()->SetActive(false);
    }

    
}

void CNpcTemple::Tick()
{
    if (!m_Child.empty())
    {
        if (m_Child[0]->GetRenderComponent()->GetActive())
        {
            if (KEY_TAP(KEY::F) && !m_Next)
            {
                m_Child[0]->Collider2D()->Activate();
                CFontMgr::GetInst()->SetActive(m_DialogStart, false);
                m_Next = true;
                m_DialogBox->GetRenderComponent()->SetActive(true);


                StateMachine()->ChangeState(L"CNpcTemD2");
            }
        }
    }
}

void CNpcTemple::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CNpcTemple::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    if (_OtherObject->Transform()->GetRelativeScale().x > 0)
    {
        m_Start = true;
        // ÇÑ¹ø¸¸ Å°°í ²¨¾ßÁö
        if (!m_Child.empty() && !m_Next)
        {
            if (m_Start && m_MainCamera->Transform()->GetRelativePos().x == 50.f)
            {
                for (int i = 0; i < m_Child.size(); ++i)
                {
                    m_Child[i]->GetRenderComponent()->SetActive(true);
                }
                CFontMgr::GetInst()->SetActive(m_DialogStart, true);
                m_Start = false;
            }
        }
    }
    else
    {
        if (!m_Child.empty())
        {
            if ((m_Child[0]->GetRenderComponent()->GetActive()))
            {
                for (int i = 0; i < m_Child.size(); ++i)
                {
                    m_Child[i]->GetRenderComponent()->SetActive(false);
                }
                CFontMgr::GetInst()->SetActive(m_DialogStart, false);
            }
        }

    }

}
       

void CNpcTemple::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    for (int i = 0; i < m_Child.size(); ++i)
    {
        m_Child[i]->GetRenderComponent()->SetActive(false);
    }
    CFontMgr::GetInst()->SetActive(m_DialogStart, false);
}
