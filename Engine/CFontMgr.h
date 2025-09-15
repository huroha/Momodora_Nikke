#pragma once

#include "singleton.h"

#include <FontEngine/FW1FontWrapper.h>
#include <FontEngine/FW1CompileSettings.h>

#ifdef _DEBUG
#pragma comment(lib, "FontEngine/FW1FontWrapper_debug.lib")
#else
#pragma comment(lib, "FontEngine/FW1FontWrapper.lib"))
#endif

#define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24 ) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)


// 출력할 텍스트의 속성을 담는 구조체
struct FontDesc
{
    wstring fullText;            // 전체 텍스트
    float posX;
    float posY;
    float fontSize;
    UINT color;
    bool isActive = true;

    bool useFadeIn = false;     // 페이드인 효과 사용 여부
    vector<std::pair<wchar_t, float>> chars;  // 페이드인 사용시: 각 글자와 알파값
    size_t currentCharIndex = 0;         // 페이드인 사용시: 현재 페이드인 중인 글자 인덱스
    bool isComplete = false;             // 페이드인 사용시: 모든 글자 페이드인 완료 여부
    float fadeSpeed = 2.0f;            // 페이드인 속도


    bool useFadeOut = false;    // 페이드아웃 사용 여부
    bool isFadingOut = false;   // 현재 페이드아웃 진행중인지
    float currentAlpha = 1.0f;  // 전체 텍스트의 알파값 (페이드아웃용)

    bool useSimultaneousFadeIn = false;  // 전체 텍스트 동시 페이드인 여부
    float simultaneousAlpha = 0.0f;      // 전체 텍스트의 알파값 (동시 페이드인용)
};


class CFontMgr :
    public singleton<CFontMgr>
{
    SINGLE(CFontMgr);
private:
    IFW1Factory* m_FW1Factory;
    IFW1FontWrapper* m_FontWrapper;

    vector<FontDesc> m_FontList;

    vector<FontDesc> m_PasueFontList;

    bool             m_PauseCheck;

public:
    void Init();
    void DrawFont(const wchar_t* pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color);

    // 새로운 텍스트 등록 함수
    int RegisterText(wstring& text, float x, float y, float size, UINT color);
    int RegisterPauseText(wstring& text, float x, float y, float size, UINT color);


    int RegisterFadeText(wstring& text, float x, float y, float size, UINT color, float fadeSpeed = 2.0f); 
    int RegisterSimultaneousFadeText(wstring& text, float x, float y, float size, UINT color, float fadeSpeed);
    // 등록된 텍스트 수정 함수
    void UpdateText(int id, wstring& newText);
    void UpdatePauseText(int id, wstring& newText);
    void UpdatePosition(int id, float x, float y);
    void UpdateTestPosition(int id, float x, float y, float z, bool _IsPause);

    void UpdateColor(int id, UINT color, bool _IsPauseFont);
    void SetActive(int id, bool active);
    void SetPauseActive(int id, bool active);
    void SetFadeActive(int id, bool active);

    void StartFadeOut(int id, float fadeSpeed = 2.0f);
    bool IsFadeOutComplete(int id);
    void SetSimultaneousFadeActive(int _id, bool active);

    void ResetFontList() { m_FontList.clear(); }
    void ResetPauseFontList() { m_PasueFontList.clear(); }
    
    void SetPause(bool _Pause) { m_PauseCheck = _Pause; }

public:
    void SetFadeSpeed(int id, float speed);
    bool IsComplete(int id)
    {
        if (id >= 0 && id < m_FontList.size())
        {
            return m_FontList[id].isComplete;
        }
        return false;
    }

   

    void Render();
};

