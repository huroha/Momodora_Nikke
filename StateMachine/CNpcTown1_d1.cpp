#include "pch.h"
#include "CNpcTown1_d1.h"
#include <Engine/CLevelMgr.h>

CNpcTown1_d1::CNpcTown1_d1()
    : m_MsgBox(nullptr)
{
}

CNpcTown1_d1::~CNpcTown1_d1()
{
}



void CNpcTown1_d1::FinalTick()
{
}

void CNpcTown1_d1::Enter()
{
    m_MsgBox = CLevelMgr::GetInst()->FindObjectByName(L"MsgBox");
    m_MsgBox->GetRenderComponent()->SetActive(false);



}

void CNpcTown1_d1::Exit()
{
    m_MsgBox->GetRenderComponent()->SetActive(true);
}