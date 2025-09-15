#pragma once

class CSound;

struct tFadeSound
{
    Ptr<CSound>    pSound;     // ���̵� ��� ����
    float          fCurTime;   // ���� ���� �ð�
    float          fEndTime;   // �� ���̵� �ð�
    float          fStartVol;  // ���� ����
    float          fEndVol;    // ��ǥ ����
    int            iChannelIdx;// ä�� �ε���
    bool           bFadeIn;    // true: FadeIn, false: FadeOut
};


struct tFXInfo
{
    float fLastPlayTime;    // ������ ��� �ð�
    float fMinInterval;     // �ּ� ��� ����
};


class CSoundMgr :
    public singleton<CSoundMgr>
{
    SINGLE(CSoundMgr);
private:
    map<wstring, Ptr<CSound>> m_mapBGMSound;
    map<wstring, Ptr<CSound>> m_mapFXSound;
    float m_fBGMVolume;
    float m_fFXVolume;
    vector<tFadeSound>    m_vecFadeSound;
    map<wstring, tFXInfo> m_mapFXInfo;     // ȿ���� ��� ����

public:
    void Init();
    void Tick();


    // ���� �ε� �Լ�
    void LoadBGM(const wstring& _strKey, const wstring& _strRelativePath);
    void LoadFX(const wstring& _strKey, const wstring& _strRelativePath);

    // ���� ��� �Լ�
    void PlayBGM(const wstring& _strKey, float _fVolume = 1.f);
    void PlayFX(const wstring& strKey, float fVolume = 1.f, float fMinInterval = 0.1f);

    // ���� ���� �Լ�
    void SetBGMVolume(float _f);
    void SetFXVolume(float _f);

    float GetBGMVolume() { return m_fBGMVolume; }
    float GetFXVolume() { return m_fFXVolume; }

    // BGM ����
    void StopBGM();

    void FadeOut(const wstring& _strBGMKey, float _fTime);  // time �� fadeout �� �ɸ��� �ð�
    void FadeIn(const wstring& _strBGMKey, float _fTime, float _fEndVolume = 1.f);
    void FadeOutAll(float _fTime);    // ��� BGM ���̵�ƿ�

};

