#pragma once


class CTimeMgr
	: public singleton<CTimeMgr>
{
	SINGLE(CTimeMgr);
private:
	LARGE_INTEGER	m_Frequency;	// 1�ʴ� ī���� ���� ��
	LARGE_INTEGER	m_CurrentCount;	// ���� ī����
	LARGE_INTEGER	m_PrevCount;	// ���� ������ ī����

	float			m_fDT;			// DeltaTime : 1�����ӿ� �ɸ� �ð�
	float			m_Time;			// ���� �ð�

	float			m_fEngineDT;
	float			m_fEngineTime;

	double			m_Second;	// 1�� üũ �뵵 �����ð�

	UINT			m_FPS;

	bool			m_IsStop;

	wchar_t			m_TimeInfo[255];

	int m_timeDisplayId;    // �ð� ǥ�ÿ� FontID
	int m_lastSecond;       // ���������� ǥ���� �ð�
	bool m_timeDisplayInitialized; // ��Ʈ �ʱ�ȭ ����

	int m_slayCountDisplayId;  // óġ �� ǥ�ÿ� FontID
	int m_lastSlayCount;      // ���������� ǥ���� óġ ��
	bool m_slayCountInitialized; // óġ �� ��Ʈ �ʱ�ȭ ����

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

	void	UpdateTimeDisplay();   // �ð� ǥ�� ������Ʈ

	void	InitTimeDisplay();     // �ð� ǥ�� �ʱ�ȭ
	void InitSlayCountDisplay();    // óġ �� ǥ�� �ʱ�ȭ
	void UpdateSlayCountDisplay();  // óġ �� ǥ�� ������Ʈ

	void FontActive(bool _Active);

public:
	float GetDeltaTime() { return m_fDT; }
	float GetEngineDeltaTime() { return m_fEngineDT; }

};