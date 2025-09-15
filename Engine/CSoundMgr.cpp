#include "pch.h"
#include "CSoundMgr.h"
#include "CAssetMgr.h"
#include "CSound.h"
#include "CTimeMgr.h"

CSoundMgr::CSoundMgr()
    : m_fBGMVolume(1.f)
    , m_fFXVolume(1.f)
{

}
CSoundMgr::~CSoundMgr()
{

}

void CSoundMgr::Init()
{
    // BGM
    LoadBGM(L"Boss_Bgm", L"Sound\\Boss_BGM.wav");
    LoadBGM(L"Temple_Bgm", L"Sound\\kohovillage.wav");
    LoadBGM(L"Lobby_Bgm", L"Sound\\Lobby_intro.wav");
    LoadBGM(L"Dungeon_Bgm", L"Sound\\DungeonBGM.mp3");

    // 효과음
    LoadFX(L"Monster2Hit", L"Sound\\BakMonkey.wav");
    LoadFX(L"Bell", L"Sound\\Bell.wav");
    LoadFX(L"BossQuake", L"Sound\\BossQuake.wav");
    LoadFX(L"BossDead", L"Sound\\Boss_Dead.wav");
    LoadFX(L"BossJump", L"Sound\\BossJump.wav");
    LoadFX(L"BossJumpDown", L"Sound\\BossJumpDown.wav");
    LoadFX(L"BossRoar", L"Sound\\BossRoar.wav");
    LoadFX(L"BossBress", L"Sound\\Bress.wav");
    LoadFX(L"MonsterHit", L"Sound\\HitSound.wav");
    LoadFX(L"PlayerJump", L"Sound\\jumppad.wav");
    LoadFX(L"HPLow", L"Sound\\lowhp.wav");
    LoadFX(L"Monster1Attack", L"Sound\\Monster1Attack.wav");
    LoadFX(L"Monster2Jump", L"Sound\\Monster2Jump.wav");
    LoadFX(L"MonsterDead", L"Sound\\MonsterDead.wav");
    LoadFX(L"PauseClose", L"Sound\\PauseClose.wav");
    LoadFX(L"PauseOpen", L"Sound\\PauseOpen.wav");
    LoadFX(L"PlayerAttack1", L"Sound\\PlayerAttack1.wav");
    LoadFX(L"PlayerAttack2", L"Sound\\PlayerAttack2.wav");
    LoadFX(L"PlayerAttack3", L"Sound\\PlayerAttack3.wav");
    LoadFX(L"PlayerDodge", L"Sound\\PlayerDodge.wav");
    LoadFX(L"PlayerLand", L"Sound\\PlayerLand.wav");
    LoadFX(L"WaterPond", L"Sound\\WaterPond.wav");
    LoadFX(L"Bust", L"Sound\\Bust.wav");
    LoadFX(L"Talking", L"Sound\\Talking.mp3");
    LoadFX(L"BustSlash", L"Sound\\BustSlash.wav");
    LoadFX(L"BombExplosion", L"Sound\\BombExplosion.wav");
    LoadFX(L"BombThrow", L"Sound\\BombThrow.wav");
    LoadFX(L"JellyJump", L"Sound\\JellyJump.wav");
    LoadFX(L"Monster3Hit", L"Sound\\Monster3_hurt.wav");



}

void CSoundMgr::Tick()
{
    vector<tFadeSound>::iterator iter = m_vecFadeSound.begin();
    while (iter != m_vecFadeSound.end())
    {
        iter->fCurTime += DT;

        float fRatio = iter->fCurTime / iter->fEndTime;
        if (fRatio >= 1.f)
        {
            if (!iter->bFadeIn)
            {
                iter->pSound->Stop();
            }
            else
            {
                // FadeIn 완료시에는 목표 볼륨으로 설정
                float fVolume = iter->fEndVol;
                int iChannelIdx = iter->iChannelIdx;
                iter->pSound->SetVolume(fVolume, iChannelIdx);
            }
            iter = m_vecFadeSound.erase(iter);
        }
        else
        {
            float fVolume;
            if (iter->bFadeIn)
                fVolume = iter->fStartVol + ((iter->fEndVol - iter->fStartVol) * fRatio);
            else
                fVolume = iter->fStartVol * (1.f - fRatio);

            int iChannelIdx = iter->iChannelIdx;
            iter->pSound->SetVolume(fVolume, iChannelIdx);
            ++iter;
        }
    }
}

void CSoundMgr::LoadBGM(const wstring& _strKey, const wstring& _strRelativePath)
{
    Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(_strRelativePath, _strRelativePath);
    m_mapBGMSound.insert(make_pair(_strKey, pSound));
}

void CSoundMgr::LoadFX(const wstring& _strKey, const wstring& _strRelativePath)
{
    Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(_strRelativePath, _strRelativePath);
    m_mapFXSound.insert(make_pair(_strKey, pSound));
}

void CSoundMgr::PlayBGM(const wstring& _strKey, float _fVolume)
{
    map<wstring, Ptr<CSound>>::iterator iter = m_mapBGMSound.find(_strKey);
    if (iter == m_mapBGMSound.end())
        return;

    // BGM은 중복재생 허용 안함, 무한반복
    iter->second->Play(0, _fVolume * m_fBGMVolume, false);
}

void CSoundMgr::PlayFX(const wstring& strKey, float fVolume, float fMinInterval)
{
    map<wstring, Ptr<CSound>>::iterator iter = m_mapFXSound.find(strKey);
    if (iter == m_mapFXSound.end())
        return;

    // 마지막 재생 시간 체크
    float fCurTime = g_Data.Time; // 현재 시간을 가져오는 함수
    auto& fxInfo = m_mapFXInfo[strKey];

    if (fCurTime - fxInfo.fLastPlayTime < fxInfo.fMinInterval)
        return;  // 최소 간격이 지나지 않았으면 재생하지 않음

    // 효과음 재생
    int iRoopCount = 1;
    float fVol = fVolume * m_fFXVolume;
    iter->second->Play(iRoopCount, fVol, true);

    // 재생 정보 업데이트
    fxInfo.fLastPlayTime = fCurTime;
    fxInfo.fMinInterval = fMinInterval;

}



void CSoundMgr::SetBGMVolume(float _f)
{
    m_fBGMVolume = _f;

    // 현재 재생중인 모든 BGM의 볼륨을 업데이트
    for (const auto& bgm : m_mapBGMSound)
    {
        if (bgm.second->IsPlaying())
        {
            bgm.second->SetVolume(m_fBGMVolume);
        }
    }

    // FadeSound 목록의 볼륨도 조정
    for (auto& fade : m_vecFadeSound)
    {
        if (fade.bFadeIn)
        {
            fade.fEndVol = fade.fEndVol / fade.pSound->GetVolume() * m_fBGMVolume;
        }
    }
}

void CSoundMgr::SetFXVolume(float _f)
{
    m_fFXVolume = _f;
}

void CSoundMgr::StopBGM()
{
    map<wstring, Ptr<CSound>>::iterator iter = m_mapBGMSound.begin();
    for (; iter != m_mapBGMSound.end(); ++iter)
    {
        iter->second->Stop();
    }
}

void CSoundMgr::FadeOut(const wstring& _strBGMKey, float _fTime)
{
    map<wstring, Ptr<CSound>>::iterator iter = m_mapBGMSound.find(_strBGMKey);
    if (iter == m_mapBGMSound.end())
        return;

    tFadeSound fade = {};
    fade.pSound = iter->second;
    fade.fEndTime = _fTime;
    fade.fStartVol = m_fBGMVolume;
    fade.fEndVol = 0.f;
    fade.bFadeIn = false;

    int iRoopCount = 0;
    fade.iChannelIdx = fade.pSound->Play(iRoopCount, m_fBGMVolume, false);

    m_vecFadeSound.push_back(fade);
}

void CSoundMgr::FadeIn(const wstring& _strBGMKey, float _fTime, float _fEndVolume)
{
    map<wstring, Ptr<CSound>>::iterator iter = m_mapBGMSound.find(_strBGMKey);
    if (iter == m_mapBGMSound.end())
        return;

    tFadeSound fade = {};
    fade.pSound = iter->second;
    fade.fEndTime = _fTime;
    fade.fStartVol = 0.f;
    fade.fEndVol = _fEndVolume * m_fBGMVolume;
    fade.bFadeIn = true;

    int iRoopCount = 0;
    float fStartVol = 0.f;
    fade.iChannelIdx = fade.pSound->Play(iRoopCount, fStartVol, false);

    m_vecFadeSound.push_back(fade);
}

void CSoundMgr::FadeOutAll(float _fTime)
{
    map<wstring, Ptr<CSound>>::iterator iter = m_mapBGMSound.begin();
    for (; iter != m_mapBGMSound.end(); ++iter)
    {
        FadeOut(iter->first, _fTime);
    }
}
