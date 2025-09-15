#include "CNpcTempleDialog.h"
#include "CNpcTempleDialog.h"
#include "CNpcTempleDialog.h"
#include "CNpcTempleDialog.h"
#include "pch.h"
#include "CNpcTempleDialog.h"

#include <Engine/CFontMgr.h>
#include "CPlayerScript.h"
#include "CNpcTemple.h"



CNpcTempleDialog::CNpcTempleDialog()
    : CScript((UINT)SCRIPT_TYPE::NPCTEMPLEDIALOG)
    , m_Dialog_1(-1)
    , m_Dialog_2(-1)
    , m_Dialog_3(-1)
    , m_Next(false)
    , m_Last(false)
{
}

CNpcTempleDialog::~CNpcTempleDialog()
{
}


void CNpcTempleDialog::SaveComponent(FILE* _File)
{
}

void CNpcTempleDialog::LoadComponent(FILE* _File)
{
}
void CNpcTempleDialog::Begin()
{
    wstring Dialog = L"악마들이 우리 마을에,";
    wstring Dialog2 = L"신성한 룬 나무에 다다르기 전에";
    wstring Dialog3= L"부디 악마들을 물리쳐주세요.";
    m_Dialog_1 = CFontMgr::GetInst()->RegisterFadeText(Dialog, 800.f, 524.f, 20.f, FONT_RGBA(0, 0, 0, 255),10.f);
    m_Dialog_2 = CFontMgr::GetInst()->RegisterFadeText(Dialog2, 800.f, 554.f, 20.f, FONT_RGBA(0, 0, 0, 255),10.f);
    m_Dialog_3 = CFontMgr::GetInst()->RegisterFadeText(Dialog3, 800.f, 540.f, 20.f, FONT_RGBA(0, 0, 0, 255), 10.f);
    CFontMgr::GetInst()->SetFadeActive(m_Dialog_1, false);
    CFontMgr::GetInst()->SetFadeActive(m_Dialog_2, false);
    CFontMgr::GetInst()->SetFadeActive(m_Dialog_3, false);

}

void CNpcTempleDialog::Tick()
{

}

void CNpcTempleDialog::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    CFontMgr::GetInst()->SetFadeActive(m_Dialog_1, true);
    const vector<CScript*>& vecScripts = _OtherObject->GetParent()->GetScripts();
    CPlayerScript* sPlayerScript = (CPlayerScript*)vecScripts[0];
    sPlayerScript->GetInfo().IsRollnAttack = true;

}

void CNpcTempleDialog::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CNpcTempleDialog::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    const vector<CScript*>& vecScripts = _OtherObject->GetParent()->GetScripts();
    CPlayerScript* sPlayerScript = (CPlayerScript*)vecScripts[0];
    sPlayerScript->GetInfo().IsRollnAttack = false;

}