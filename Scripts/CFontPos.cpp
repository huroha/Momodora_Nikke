#include "pch.h"
#include "CFontPos.h"
#include <Engine/CFontMgr.h>


CFontPos::CFontPos()
    :CScript(SCRIPT_TYPE::FONTPOS)
    , m_TestID(-1)
{
}

CFontPos::~CFontPos()
{
}

void CFontPos::SaveComponent(FILE* _File)
{
}

void CFontPos::LoadComponent(FILE* _File)
{
}

void CFontPos::Begin()
{
    wstring Dialog = L"Test Font";
    m_TestID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 0.f, 0.f, 26.f, FONT_RGBA(255, 255, 255, 255));
   
}

void CFontPos::Tick()
{
    float xPos = Transform()->GetRelativePos().x;
    float yPos = Transform()->GetRelativePos().y;
    float fontSize = Transform()->GetRelativePos().z;

    CFontMgr::GetInst()->UpdateTestPosition(m_TestID, xPos, yPos, fontSize, true);

}