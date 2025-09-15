#include "pch.h"
#include "CFlipbookPlayer.h"

#include "CTimeMgr.h"
#include "CConstBuffer.h"
#include "CDevice.h"

CFlipbookPlayer::CFlipbookPlayer()
	: CComponent(COMPONENT_TYPE::FLIPBOOKPLAYER)
	, m_SpriteIdx(0)
	, m_Repeat(false)
	, m_FPS(24)
	, m_Time(0.f)
	, m_FlipbookCount(0)
	, m_IsHit(false)
	, m_HitEffectTime(0.f)
	, m_HitDuration(0.f)
{
}

CFlipbookPlayer::~CFlipbookPlayer()
{
}

void CFlipbookPlayer::FinalTick()
{
	//현재 진행중인 Flipbook이 없거나. 지정한 Flipbook이 한차례 돌았고, 반복재생을 지정해주지 않았다면 아무런 역할을 하지 않는다.
	if (nullptr == m_CurFlipbook || (m_Finish && !m_Repeat))
		return;

	// 히트 효과 업데이트
	UpdateHitEffect();

	// 한차례 끝났고, 반복설정을 했다면, 재생할 Flipbook idx 초기화
	if (m_Finish && m_Repeat)
	{
		m_SpriteIdx = 0;
		m_Finish = false;
	}

	// 스프라이트 프레임 간격 시간
	float Term = (1.f / m_FPS);

	if (Term < m_Time)
	{
		++m_SpriteIdx;

		// Sprite 인덱스 초과시 마지막 Sprite를 재생시킴.
		if (m_CurFlipbook->GetMaxSprite() <= m_SpriteIdx)
		{
			m_SpriteIdx -= 1;
			m_Finish = true;
		}

		m_Time -= Term;
	}

	m_Time += DT;
}

void CFlipbookPlayer::AddFlipbook(int _idx, Ptr<CFlipbook> _Flipbook)
{
	if (m_vecFlipbook.size() <= _idx)
	{
		m_vecFlipbook.resize(_idx + 1);
	}

	m_vecFlipbook[_idx] = _Flipbook;
	m_FlipbookCount += 1;
}

void CFlipbookPlayer::RemoveFlipbook()
{
	if (m_vecFlipbook.size() < 1)
		return;
	m_vecFlipbook[m_vecFlipbook.size() - 1] = nullptr;
	m_CurFlipbook = m_vecFlipbook[0];
	m_FlipbookCount -= 1;
}

void CFlipbookPlayer::Binding()
{
	if (nullptr == m_CurFlipbook)
		return;

	Ptr<CSprite> Sprite = m_CurFlipbook->GetSprite(m_SpriteIdx);
	if (nullptr != Sprite)
	{
		// 히트 효과 정보를 상수 버퍼에 직접 설정
		static CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::SPRITE);
		SpriteInfo info = {};

		// 기본 스프라이트 정보 설정
		info.LeftTopUV = Sprite->GetLeftTopUV();
		info.SliceUV = Sprite->GetSliceUV();
		info.BackgroundUV = Sprite->GetBackgroundUV();
		info.OffsetUV = Sprite->GetOffsetUV();
		info.SpriteUse = true;

		// 히트 효과 정보 설정
		info.UseHitEffect = m_IsHit ? 1 : 0;
		info.HitEffectRatio = CalculateHitRatio();
		info.HitColor = m_HitColor;

		// 텍스쳐만 바인딩
		if (Sprite->GetAtlasTexture().Get())
			Sprite->GetAtlasTexture()->Binding(12);

		pCB->SetData(&info);
		pCB->Binding();
	}
}

void CFlipbookPlayer::Clear()
{
	if (nullptr == m_CurFlipbook)
		return;

	CSprite::Clear();
}

// 다른 곳에서 사용할 때 쓰임
void CFlipbookPlayer::StartHitEffect(float _duration, Vec3 hitColor)
{
	m_IsHit = true;
	m_HitEffectTime = 0.f;
	m_HitDuration = _duration;
	m_HitColor = hitColor;
}



void CFlipbookPlayer::UpdateHitEffect()
{
	if (m_IsHit)
	{
		m_HitEffectTime += DT;
		if (m_HitEffectTime >= m_HitDuration)
		{
			m_IsHit = false;
			m_HitEffectTime = 0.f;
		}
	}

}

void CFlipbookPlayer::SaveComponent(FILE* _File)
{
	fwrite(&m_SpriteIdx, sizeof(int), 1, _File);
	fwrite(&m_Repeat, sizeof(bool), 1, _File);
	fwrite(&m_FPS, sizeof(float), 1, _File);
	fwrite(&m_FlipbookCount, sizeof(int), 1, _File);

	size_t count = m_vecFlipbook.size();
	fwrite(&count, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < m_vecFlipbook.size(); ++i)
	{
		SaveAssetRef(m_vecFlipbook[i], _File);
	}

	SaveAssetRef(m_CurFlipbook, _File);
}

void CFlipbookPlayer::LoadComponent(FILE* _File)
{
	fread(&m_SpriteIdx, sizeof(int), 1, _File);
	fread(&m_Repeat, sizeof(bool), 1, _File);
	fread(&m_FPS, sizeof(float), 1, _File);
	fread(&m_FlipbookCount, sizeof(int), 1, _File);

	size_t count = 0;
	fread(&count, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < count; ++i)
	{
		Ptr<CFlipbook> pFlipBook;
		LoadAssetRef(pFlipBook, _File);
		m_vecFlipbook.push_back(pFlipBook);
	}

	LoadAssetRef(m_CurFlipbook, _File);
}