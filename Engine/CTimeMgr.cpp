#include "pch.h"
#include "CTimeMgr.h"

#include "CEngine.h"
#include "CFontMgr.h"
#include "CKeyMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CSoundMgr.h"
#include "CDataMgr.h"

int g_Slay_Count = 0;

CTimeMgr::CTimeMgr()
	: m_Frequency{}
	, m_CurrentCount{}
	, m_PrevCount{}
	, m_fDT(0.f)
	, m_Time(0.f)
	, m_fEngineDT(0.f)
	, m_fEngineTime(0.f)
	, m_FPS(0)
	, m_Second(0.)
	, m_IsStop(true)
	, m_TimeInfo{}
	, m_timeDisplayInitialized(false)
	, m_lastSecond(-1)
	, m_timeDisplayId(-1)
	, m_slayCountDisplayId(-1)
	, m_lastSlayCount(-1)
	, m_slayCountInitialized(false)
	, m_FontActive(true)
{

}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::Init()
{
	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_PrevCount);
}

void CTimeMgr::Tick()
{
	QueryPerformanceCounter(&m_CurrentCount);

	m_fEngineDT = (float)(m_CurrentCount.QuadPart - m_PrevCount.QuadPart) / (float)m_Frequency.QuadPart;

	// DT 보정, 60 분의 1 보다 크지 못하게 보정
	if ((1.f / 60.f) < m_fEngineDT)
		m_fEngineDT = (1.f / 60.f);

	// 시간 누적
	m_fEngineTime += m_fEngineDT;

	// 시간 누적
	m_Second += m_fEngineDT;

	// 함수 호출횟수
	++m_FPS;

	if (1. < m_Second)
	{
		m_Second -= 1.;

		// 윈도우 타이틀에 FPS 랑 DeltaTime 표시
		// 윈도우 타이틀에 FPS 랑 DeltaTime 표시
		swprintf_s(m_TimeInfo, L"DeltaTime : %f, FPS : %d ", m_fEngineDT, m_FPS);
		m_FPS = 0;
	}

	m_PrevCount = m_CurrentCount;

	if (!m_IsStop)
	{
		m_fDT = m_fEngineDT;
		m_Time += m_fDT;
	}

	// GlobalData 갱신
	g_Data.DeltaTime = m_fDT;
	g_Data.Time = m_Time;

	g_Data.DT_Engine = m_fEngineDT;
	g_Data.Time_Engine = m_fEngineTime;


	// 시간 표시 업데이트
	UpdateTimeDisplay();

	// 처치 수 표시 업데이트
	UpdateSlayCountDisplay();

	if (KEY_TAP(KEY::ESC))
	{
		if (CLevelMgr::GetInst()->GetCurrentLevel()->GetState() == LEVEL_STATE::PLAY)
		{
			ChangeLevelState(LEVEL_STATE::PAUSE);
			CSoundMgr::GetInst()->PlayFX(L"PauseOpen", 0.5f);
		}
		else
		{
			ChangeLevelState(LEVEL_STATE::PLAY);
			CSoundMgr::GetInst()->PlayFX(L"PauseClose", 0.5f);
		}
	}

}

void CTimeMgr::Render()
{
	CFontMgr::GetInst()->DrawFont(m_TimeInfo, 10, 20, 16, FONT_RGBA(255, 20, 20, 255));

}

void CTimeMgr::InitTimeDisplay()
{

		// 시간 표시용 텍스트 등록
		wstring timeStr = L"0 : 0";
		m_timeDisplayId = CFontMgr::GetInst()->RegisterPauseText(timeStr, 674.f, 252.f, 26.f, FONT_RGBA(255, 247, 215, 255));
		m_timeDisplayInitialized = true;
}

void CTimeMgr::UpdateTimeDisplay()
{

	int second = (int)g_Data.Time_Engine % 60;
	int min1 = (int)g_Data.Time_Engine / 60;

		
	// 시간이 변경되었을 때만 업데이트
	if (second != m_lastSecond)
	{
		wstring timeStr;
		if (min1 < 10)
			timeStr = L"0" + std::to_wstring(min1) + L" : " + std::to_wstring(second);
		else
			timeStr = std::to_wstring(min1) + L" : " + std::to_wstring(second);
		
		CFontMgr::GetInst()->UpdatePauseText(m_timeDisplayId, timeStr);
		m_lastSecond = second;
	}
}

void CTimeMgr::InitSlayCountDisplay()
{

		// 처치 수 표시용 텍스트 등록

		
		wstring countStr = std::to_wstring(CDataMgr::GetInst()->GetPlayerData().SlayCount);
		m_slayCountDisplayId = CFontMgr::GetInst()->RegisterPauseText(countStr, 656.f, 306.f, 26.f, FONT_RGBA(255, 247, 215, 255));
		m_slayCountInitialized = true;

}

void CTimeMgr::UpdateSlayCountDisplay()
{

	// 처치 수가 변경되었을 때만 업데이트
	if (CDataMgr::GetInst()->GetPlayerData().SlayCount != m_lastSlayCount)
	{
		wstring countStr = std::to_wstring(CDataMgr::GetInst()->GetPlayerData().SlayCount);
		CFontMgr::GetInst()->UpdatePauseText(m_slayCountDisplayId, countStr);
		m_lastSlayCount = CDataMgr::GetInst()->GetPlayerData().SlayCount;
	}
}

void CTimeMgr::FontActive(bool _Active)
{
	if (_Active)
	{
		CFontMgr::GetInst()->SetPauseActive(m_timeDisplayId, _Active);
		CFontMgr::GetInst()->SetPauseActive(m_slayCountDisplayId, _Active);
	}
	else
	{
		CFontMgr::GetInst()->SetPauseActive(m_timeDisplayId, _Active);
		CFontMgr::GetInst()->SetPauseActive(m_slayCountDisplayId, _Active);
	}
}
