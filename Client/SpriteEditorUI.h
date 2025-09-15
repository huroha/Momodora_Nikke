#pragma once
#include "EditorUI.h"
#include <Engine/CTexture.h>

// 스프라이트 에디터 UI 클래스
// - 텍스처 아틀라스에서 스프라이트를 편집하고 생성하는 기능을 제공
class SpriteEditorUI
    : public EditorUI
{
private:
    // =========================================
    // 텍스처 관련 멤버 변수
    // =========================================
    Ptr<CTexture>   m_Atlas;                            // 현재 편집중인 아틀라스 텍스처
    ImVec2          m_TextureDisplayPos;                // 텍스처가 실제로 표시되는 화면상의 위치
    float           m_ZoomLevel;                        // 현재 줌 레벨 (1.0f가 기본값)

    // =========================================
    // 선택 영역 관련 멤버 변수
    // =========================================
    Vec2            m_LeftTop;                          // 선택된 영역의 좌상단 좌표 (픽셀)
    Vec2            m_Slice;                            // 선택된 영역의 크기 (픽셀)
    Vec2            m_Background;                       // 배경 영역 크기 (픽셀)
    Vec2            m_Offset;                           // 오프셋 값 (픽셀)
    bool            m_HasSelection;                     // 선택 영역이 있는지 여부

    // =========================================
    // 드래그 관련 멤버 변수
    // =========================================
    bool            m_bDragging;                        // 드래그 중인지 여부
    Vec2            m_DragStart;                        // 드래그 시작 위치 (텍스처 좌표)
    Vec2            m_DragCurrent;                      // 현재 드래그 위치 (텍스처 좌표)

    // =========================================
    // 패닝(이미지 이동) 관련 멤버 변수
    // =========================================
    bool            m_bPanning;                         // 패닝 중인지 여부
    Vec2            m_PanOffset;                        // 현재 패닝 오프셋
    ImVec2          m_LastMousePos;                     // 이전 마우스 위치

    // =========================================
    // UI 관련 멤버 변수
    // =========================================
    ImVec2          m_ImageMin;                         // 이미지 표시 영역의 좌상단 (스크린 좌표)
    ImVec2          m_ImageMax;                         // 이미지 표시 영역의 우하단 (스크린 좌표)
    ImVec2          m_PreviewSize;                      // 프리뷰 이미지 크기
    bool            m_MouseOverAtlas;                   // 마우스가 Atlas 영역 위에 있는지

    // =========================================
    // 스프라이트 이름 관련 멤버 변수
    // =========================================
    static char    m_SpriteNameBuf[50];                 // 스프라이트 이름 입력 버퍼
    bool           m_ShowNameError;                     // 이름 에러 팝업 표시 여부

public:
    // 생성자/소멸자
    SpriteEditorUI();
    ~SpriteEditorUI();

    // EditorUI 가상함수 오버라이드
    virtual void Tick() override {};                    // 매 프레임 갱신
    virtual void Render() override;                     // UI 렌더링
    virtual void Render_Update() override {};           // 변경사항 갱신

private:
    // =========================================
    // UI 렌더링 관련 함수
    // =========================================
    void RenderAtlasView();                             // 아틀라스 텍스처 뷰 렌더링
    void RenderControlPanel();                          // 제어 패널 렌더링
    void RenderPreview();                               // 프리뷰 영역 렌더링
    void RenderSelectionRect();                         // 선택 영역 렌더링
    void RenderNameErrorPopup();                        // 이름 에러 팝업 렌더링

    // =========================================
    // 입력 처리 관련 함수
    // =========================================
    void HandleDragging();                              // 드래그 처리
    void HandlePanning();                               // 패닝(우클릭 드래그) 처리

    // =========================================
    // 선택 영역 관련 함수
    // =========================================
    void UpdateSelectionFromDrag();                     // 드래그 정보로 선택 영역 업데이트
    void UpdateSelectionRect();                         // 선택 영역 제한 및 업데이트

    // =========================================
    // 스프라이트 생성 관련 함수
    // =========================================
    void CreateSprite(const wstring& _Name);            // 스프라이트 생성
    wstring GetUniqueSpriteNameFromInput() const;       // 입력된 이름으로 고유한 스프라이트 이름 생성

    // =========================================
    // 유틸리티 함수
    // =========================================
    void LoadTextureFromFile();                         // 파일에서 텍스처 로드
    wstring OpenFileDialog(const wchar_t* filter);      // 파일 다이얼로그 표시
    Vec2 GetAdjustedDisplayPos() const;                 // 패닝이 적용된 디스플레이 위치 계산
    Vec2 ScreenToTextureSpace(const ImVec2& screenPos); // 화면 좌표를 텍스처 좌표로 변환
    bool IsInImageBounds(const ImVec2& point);          // 좌표가 이미지 영역 내에 있는지 확인


};