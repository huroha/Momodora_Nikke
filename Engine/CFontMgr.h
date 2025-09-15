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


// ����� �ؽ�Ʈ�� �Ӽ��� ��� ����ü
struct FontDesc
{
    wstring fullText;            // ��ü �ؽ�Ʈ
    float posX;
    float posY;
    float fontSize;
    UINT color;
    bool isActive = true;

    bool useFadeIn = false;     // ���̵��� ȿ�� ��� ����
    vector<std::pair<wchar_t, float>> chars;  // ���̵��� ����: �� ���ڿ� ���İ�
    size_t currentCharIndex = 0;         // ���̵��� ����: ���� ���̵��� ���� ���� �ε���
    bool isComplete = false;             // ���̵��� ����: ��� ���� ���̵��� �Ϸ� ����
    float fadeSpeed = 2.0f;            // ���̵��� �ӵ�


    bool useFadeOut = false;    // ���̵�ƿ� ��� ����
    bool isFadingOut = false;   // ���� ���̵�ƿ� ����������
    float currentAlpha = 1.0f;  // ��ü �ؽ�Ʈ�� ���İ� (���̵�ƿ���)

    bool useSimultaneousFadeIn = false;  // ��ü �ؽ�Ʈ ���� ���̵��� ����
    float simultaneousAlpha = 0.0f;      // ��ü �ؽ�Ʈ�� ���İ� (���� ���̵��ο�)
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

    // ���ο� �ؽ�Ʈ ��� �Լ�
    int RegisterText(wstring& text, float x, float y, float size, UINT color);
    int RegisterPauseText(wstring& text, float x, float y, float size, UINT color);


    int RegisterFadeText(wstring& text, float x, float y, float size, UINT color, float fadeSpeed = 2.0f); 
    int RegisterSimultaneousFadeText(wstring& text, float x, float y, float size, UINT color, float fadeSpeed);
    // ��ϵ� �ؽ�Ʈ ���� �Լ�
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

