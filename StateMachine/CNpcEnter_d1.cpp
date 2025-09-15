#include "pch.h"
#include "CNpcEnter_d1.h"
#include <Engine/CLevelMgr.h>


CNpcEnter_d1::CNpcEnter_d1()
    : m_MsgBox(nullptr)
{
}

CNpcEnter_d1::~CNpcEnter_d1()
{
}

void CNpcEnter_d1::FinalTick()
{
}

void CNpcEnter_d1::Enter()
{
    m_MsgBox = CLevelMgr::GetInst()->FindObjectByName(L"MsgBox");
    m_MsgBox->GetRenderComponent()->SetActive(false);



}

void CNpcEnter_d1::Exit()
{
    m_MsgBox->GetRenderComponent()->SetActive(true);
}
