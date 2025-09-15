// FlipbookEditorUI.h
#pragma once
#include "EditorUI.h"
#include <Engine/CFlipbook.h>
#include <Engine/CSprite.h>

class FlipbookEditorUI 
	: public EditorUI
{
private:
	// 현재 편집중인 플립북 에셋
	Ptr<CFlipbook>          m_EditFlipbook;					// 편집중인 플립북
	int                     m_SelectedIdx;					// 선택된 스프라이트 인덱스

	// 플레이어 프리뷰 관련
	bool                    m_bPlaying;						// 재생 중인지 여부
	float                   m_AccTime;						// 현재 프레임 누적 시간
	float                   m_FPS;							// 재생 속도 (프레임/초)
	bool                    m_Repeat;						// 반복 재생 여부
	float					m_PreviewZoom;


	// UI 상태
	bool                    m_ShowNameError;				// 이름 에러 팝업 표시 여부
	char                    m_NameBuf[50];					// 플립북 이름 입력 버퍼

	int						m_InsertIndex;					// 스프라이트 삽입 위치 (-1: 끝에 추가)

public:
	// EditorUI 가상함수 재정의
	virtual void Tick() override;							// 프레임 업데이트
	virtual void Render() override;							// UI 렌더링
	virtual void Render_Update() override {};				// 변경사항 갱신

private:
	// Flipbook 불러오기 관련 함수들
	void ShowFlipbookSelectUI();							// Flipbook 선택 UI 표시
	void LoadFlipbook(Ptr<CFlipbook> _pFlipbook);			// 선택된 Flipbook 로드

	// === Flipbook 에셋 편집 UI ===
	void RenderSpriteList();								// 스프라이트 목록
	void RenderSpriteProperties();							// 선택된 스프라이트 속성

	// === 플레이어 프리뷰 UI ===
	void RenderPlayerControls();							// 플레이어 제어
	void RenderPreviewDisplay();

	// === 공통 UI ===
	void RenderNameErrorPopup();							// 이름 에러 팝업

	// === 스프라이트 관리 ===
	void ShowSpriteSelectUI();								// 스프라이트 선택 UI 표시
	void SelectSprite(void* _ptr);							// 스프라이트 선택 콜백
	void AddSprite(Ptr<CSprite> _pSprite);					// 스프라이트 추가
	void RemoveSprite(int _idx);							// 스프라이트 제거
	void MoveSprite(int _from, int _to);					// 스프라이트 순서 변경

	// === 저장/불러오기 ===
	void SaveFlipbook();									// 플립북 저장
	wstring GetUniqueFlipbookName() const;					// 고유한 플립북 이름 생성

	// === 유틸리티 ===
	void UpdatePreview();									// 프리뷰 업데이트
	void PlayPreview();										// 프리뷰 재생
	void StopPreview();										// 프리뷰 정지
	wstring OpenFileDialog(const wchar_t* filter);


public:
	FlipbookEditorUI();
	~FlipbookEditorUI();
};