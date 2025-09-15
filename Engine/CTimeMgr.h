#pragma once


class CTimeMgr
	: public singleton<CTimeMgr>
{
	SINGLE(CTimeMgr);
private:
	LARGE_INTEGER	m_Frequency;	// 1초당 카운팅 가능 수
	LARGE_INTEGER	m_CurrentCount;	// 현재 카운팅
	LARGE_INTEGER	m_PrevCount;	// 이전 프레임 카운팅

	float			m_fDT;			// DeltaTime : 1프레임에 걸린 시간
	float			m_Time;			// 누적 시간

	float			m_fEngineDT;
	float			m_fEngineTime;

	double			m_Second;	// 1초 체크 용도 누적시간

	UINT			m_FPS;

	bool			m_IsStop;

	wchar_t			m_TimeInfo[255];

	int m_timeDisplayId;    // 시간 표시용 FontID
	int m_lastSecond;       // 마지막으로 표시한 시간
	bool m_timeDisplayInitialized; // 폰트 초기화 여부

	int m_slayCountDisplayId;  // 처치 수 표시용 FontID
	int m_lastSlayCount;      // 마지막으로 표시한 처치 수
	bool m_slayCountInitialized; // 처치 수 폰트 초기화 여부

	bool	m_FontActive;

public:
	void Init();
	void Tick();
	void Render();
	void SetStopMode(bool _IsStop)
	{
		m_IsStop = _IsStop;
		if (m_IsStop)
			m_fDT = 0.f;
	}

	void	UpdateTimeDisplay();   // 시간 표시 업데이트

	void	InitTimeDisplay();     // 시간 표시 초기화
	void InitSlayCountDisplay();    // 처치 수 표시 초기화
	void UpdateSlayCountDisplay();  // 처치 수 표시 업데이트

	void FontActive(bool _Active);

public:
	float GetDeltaTime() { return m_fDT; }
	float GetEngineDeltaTime() { return m_fEngineDT; }

};