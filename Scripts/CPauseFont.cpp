#include "pch.h"
#include "CPauseFont.h"

#include <Engine/CFontMgr.h>
#include <Engine/CTimeMgr.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CSoundMgr.h>

CPauseFont::CPauseFont()
    : CScript((UINT)SCRIPT_TYPE::PAUSEFONT)
    , m_ActiveCheck(false)
    , m_BgmVolumeId(-1)
    , m_FxVolumeId(-1)
{
}

CPauseFont::~CPauseFont()
{
}



void CPauseFont::SaveComponent(FILE* _File)
{
}

void CPauseFont::LoadComponent(FILE* _File)
{
}

void CPauseFont::Begin()
{

    CreateMainFont();
    CreateOptionFont();
    CreateMemoFont();

    StateMachine()->ChangeState(L"CPauseMain");




}

void CPauseFont::Tick()
{
    if (StateMachine()->GetCurStateName() == L"CPauseOption")
    {
        if (!m_ActiveCheck)
        {
            CFontMgr::GetInst()->SetPauseActive(m_vecPauseFont[3], false);
            CFontMgr::GetInst()->SetPauseActive(m_vecPauseFont[4], false);
            CTimeMgr::GetInst()->FontActive(false);

            for (int i = 0; i < m_OptionId.size(); ++i)
            {
                CFontMgr::GetInst()->SetPauseActive(m_OptionId[i], true);
            }
            m_ActiveCheck = true;
        }
    }
    else if (StateMachine()->GetCurStateName() == L"CPauseMain")
    {
        if (!m_ActiveCheck)
        {
            CFontMgr::GetInst()->SetPauseActive(m_vecPauseFont[3], true);
            CFontMgr::GetInst()->SetPauseActive(m_vecPauseFont[4], true);
            // Time�� Slay Count active �����̴�.
            CTimeMgr::GetInst()->FontActive(true);

            for (int i = 0; i < m_OptionId.size(); ++i)
            {
                CFontMgr::GetInst()->SetPauseActive(m_OptionId[i], false);
            }
            m_ActiveCheck = true;
        }
    }
    else
    {
        if (!m_ActiveCheck)
        {
            CFontMgr::GetInst()->SetPauseActive(m_vecPauseFont[3], false);
            CFontMgr::GetInst()->SetPauseActive(m_vecPauseFont[4], false);
            CTimeMgr::GetInst()->FontActive(false);
            

            m_ActiveCheck = true;
        }


    }
}

void CPauseFont::CreateMainFont()
{

    int FontID = 0;
    wstring Dialog = L"�޸�";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 124.f, 46.f, 36.f, FONT_RGBA(255, 255, 255, 255));
    m_vecPauseFont.push_back(FontID);
    Dialog = L"���� ����";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 560.f, 46.f, 36.f, FONT_RGBA(255, 255, 255, 255));
    m_vecPauseFont.push_back(FontID);
    Dialog = L"�ɼ�";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 1080.f, 46.f, 36.f, FONT_RGBA(255, 255, 255, 255));
    m_vecPauseFont.push_back(FontID);

    Dialog = L"�÷��� �ð� : ";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 504.f, 252.f, 24.f, FONT_RGBA(255, 247, 215, 255));
    m_vecPauseFont.push_back(FontID);

    Dialog = L"óġ�� �� : ";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 504.f, 306.f, 24.f, FONT_RGBA(255, 247, 215, 255));
    m_vecPauseFont.push_back(FontID);

    Dialog = L"�޴� ����";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 88.f, 718.f, 22.f, FONT_RGBA(255, 255, 255, 255));
    m_vecPauseFont.push_back(FontID);

    Dialog = L"����";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 280.f, 718.f, 22.f, FONT_RGBA(255, 255, 255, 255));
    m_vecPauseFont.push_back(FontID);

    Dialog = L"���� �簳";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 400.f, 718.f, 22.f, FONT_RGBA(255, 255, 255, 255));
    m_vecPauseFont.push_back(FontID);


}

void CPauseFont::CreateOptionFont()
{
    int FontID = 0;
    wstring Dialog;
    Dialog = L"BGM ���� : ";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 300.f, 26.f, FONT_RGBA(255, 247, 215, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_OptionId.push_back(FontID);
    Dialog = L"ȿ���� ���� :";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 340.f, 26.f, FONT_RGBA(255, 247, 215, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_OptionId.push_back(FontID);

    // �� ID�� ���� �ѱ� �ʿ䰡 ����.
    int BgmVol = CSoundMgr::GetInst()->GetBGMVolume() * 100;
    Dialog = to_wstring(BgmVol);
    m_BgmVolumeId = CFontMgr::GetInst()->RegisterPauseText(Dialog, 760.f, 300.f, 26.f, FONT_RGBA(255, 247, 215, 255));
    CFontMgr::GetInst()->SetPauseActive(m_BgmVolumeId, false);
    m_OptionId.push_back(m_BgmVolumeId);

    int FxVol = CSoundMgr::GetInst()->GetFXVolume() * 100;
    Dialog = to_wstring(FxVol);
    m_FxVolumeId = CFontMgr::GetInst()->RegisterPauseText(Dialog, 760.f, 340.f, 26.f, FONT_RGBA(255, 247, 215, 255));
    CFontMgr::GetInst()->SetPauseActive(m_FxVolumeId, false);
    m_OptionId.push_back(m_FxVolumeId);

}

void CPauseFont::CreateMemoFont()
{
    int FontID = 0;
    wstring Dialog;

    float temp = 50.f;
    Dialog = L"ü��, ����";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 196.f, 244.f, 30.f, FONT_RGBA(100, 100, 100, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 244.f, 30.f, FONT_RGBA(250, 237, 130, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    Dialog = L"ȭ�� ���� ����� �ʷϻ� �������� ü������,\nü���� ��ġȭ�� ���� ���� ǥ�õ˴ϴ�.ü���� 0�� �Ǹ�\n�������� �й��ϸ� ���� ������� ���ư��ϴ�.\n\n������ ü�� ������ �Ʒ��� �ִ� �Ķ���\n�������� ǥ�õ˴ϴ�.";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 286.f, 26.f, FONT_RGBA(255, 247, 215, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);


    Dialog = L"����";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 196.f, 244.f + temp, 30.f, FONT_RGBA(100, 100, 100, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 244.f, 30.f, FONT_RGBA(250, 237, 130, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    Dialog = L"��� Ű : Space Bar\n\n������ ���� ���� �پ������ �⺻ �����Դϴ�.";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 286.f, 26.f, FONT_RGBA(255, 247, 215, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    Dialog = L"���� ����";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 196.f, 244.f+ temp*2, 30.f, FONT_RGBA(100, 100, 100, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 244.f, 30.f, FONT_RGBA(250, 237, 130, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    Dialog = L"��� Ű : A\n\n�÷��̾ �ٶ󺸴� �������� �����մϴ�.\n��ư�� ���޾� ������ �޺��� ����մϴ�.";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 286.f, 26.f, FONT_RGBA(255, 247, 215, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    Dialog = L"�̵��� ȸ��";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 196.f, 244.f+ temp*3, 30.f, FONT_RGBA(100, 100, 100, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 244.f, 30.f, FONT_RGBA(250, 237, 130, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    Dialog = L"��� Ű : ȭ��ǥ & X\n\n�¿� �̵��� �����⸦ ���� ȸ�Ǹ� �� �� �ֽ��ϴ�.\n������� ������ �ƴ� �����Ȳ������ ����� �� �ֽ��ϴ�.\n\n���Ϳ� ������ ���ؾ��� ����� �����մϴ�.";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 286.f, 26.f, FONT_RGBA(255, 247, 215, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    Dialog = L"Ư�� ��ų : ����Ʈ";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 196.f, 244.f + temp * 4, 30.f, FONT_RGBA(100, 100, 100, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 244.f, 30.f, FONT_RGBA(250, 237, 130, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    Dialog = L"��ȣ�ۿ� Ű : R\n\n������ 100 �Ҹ��Ͽ� 10�ʰ� ����Ʈ ���°� �˴ϴ�.\n�⺻ ������ ��ȭ�Ǹ� ������ ������\n��ü�������� ��ȭ�˴ϴ�.\n\n������ ���� Ÿ�ݽ� ȸ�� �� �� �ֽ��ϴ�.";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 286.f, 26.f, FONT_RGBA(255, 247, 215, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);


    Dialog = L"���� ���";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 196.f, 244.f+ temp*5, 30.f, FONT_RGBA(100, 100, 100, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 244.f, 30.f, FONT_RGBA(250, 237, 130, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);

    Dialog = L"��ȣ�ۿ� Ű : ����\n\n�ż��� ���� ��� ����°� ������ ������\nȸ���մϴ�. �������� �й� �� �׻� ����������\n�︰ �ż��� ������ ���ư��ϴ�.";
    FontID = CFontMgr::GetInst()->RegisterPauseText(Dialog, 540.f, 286.f, 26.f, FONT_RGBA(255, 247, 215, 255));
    CFontMgr::GetInst()->SetPauseActive(FontID, false);
    m_MemoId.push_back(FontID);


}





