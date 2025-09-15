#pragma once

class CSound;

struct tFadeSound
{
    Ptr<CSound>    pSound;     // 페이드 대상 사운드
    float          fCurTime;   // 현재 진행 시간
    float          fEndTime;   // 총 페이드 시간
    float          fStartVol;  // 시작 볼륨
    float          fEndVol;    // 목표 볼륨
    int            iChannelIdx;// 채널 인덱스
    bool           bFadeIn;    // true: FadeIn, false: FadeOut
};


struct tFXInfo
{
    float fLastPlayTime;    // 마지막 재생 시간
    float fMinInterval;     // 최소 재생 간격
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
    map<wstring, tFXInfo> m_mapFXInfo;     // 효과음 재생 정보

public:
    void Init();
    void Tick();


    // 사운드 로딩 함수
    void LoadBGM(const wstring& _strKey, const wstring& _strRelativePath);
    void LoadFX(const wstring& _strKey, const wstring& _strRelativePath);

    // 사운드 재생 함수
    void PlayBGM(const wstring& _strKey, float _fVolume = 1.f);
    void PlayFX(const wstring& strKey, float fVolume = 1.f, float fMinInterval = 0.1f);

    // 볼륨 조절 함수
    void SetBGMVolume(float _f);
    void SetFXVolume(float _f);

    float GetBGMVolume() { return m_fBGMVolume; }
    float GetFXVolume() { return m_fFXVolume; }

    // BGM 정지
    void StopBGM();

    void FadeOut(const wstring& _strBGMKey, float _fTime);  // time 은 fadeout 에 걸리는 시간
    void FadeIn(const wstring& _strBGMKey, float _fTime, float _fEndVolume = 1.f);
    void FadeOutAll(float _fTime);    // 모든 BGM 페이드아웃

};

