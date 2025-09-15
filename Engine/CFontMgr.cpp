#include "pch.h"
#include "CFontMgr.h"

#include "CDevice.h"
#include "CTimeMgr.h"

CFontMgr::CFontMgr()
	: m_FW1Factory(nullptr)
	, m_FontWrapper(nullptr)
	, m_PauseCheck(false)
{
}

CFontMgr::~CFontMgr()
{
	if (nullptr != m_FW1Factory)
		m_FW1Factory->Release();

	if (nullptr != m_FontWrapper)
		m_FontWrapper->Release();

}

void CFontMgr::Init()
{
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_FW1Factory)))
	{
		assert(nullptr);
	}

	if (FAILED(m_FW1Factory->CreateFontWrapper(DEVICE, L"DungGeunMo", &m_FontWrapper)))
	{
		assert(nullptr);
	}
}

void CFontMgr::DrawFont(const wchar_t* pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
{
	m_FontWrapper->DrawString(
		CONTEXT,
		pStr,            // String
		_fFontSize,       // Font size
		_fPosX,          // X position
		_fPosY,          // Y position
		_Color,         // Text color, 0xAaBbGgRr
		FW1_RESTORESTATE
	);
}


void CFontMgr::SetFadeSpeed(int id, float speed)
{
	if (id >= 0 && id < m_FontList.size())
	{
		m_FontList[id].fadeSpeed = speed;
	}
}


int CFontMgr::RegisterText(wstring& text, float x, float y, float size, UINT color)
{
	FontDesc desc;
	desc.fullText = text;
	desc.posX = x;
	desc.posY = y;
	desc.fontSize = size;
	desc.color = color;
	desc.useFadeIn = false;  // 페이드인 사용하지 않음

	m_FontList.push_back(desc);
	return static_cast<int>(m_FontList.size() - 1);
}

int CFontMgr::RegisterPauseText(wstring& text, float x, float y, float size, UINT color)
{

	FontDesc desc;
	desc.fullText = text;
	desc.posX = x;
	desc.posY = y;
	desc.fontSize = size;
	desc.color = color;
	desc.useFadeIn = false;

	m_PasueFontList.push_back(desc);
	return static_cast<int>(m_PasueFontList.size() - 1);
}

int CFontMgr::RegisterFadeText(wstring& text, float x, float y, float size, UINT color, float fadeSpeed)
{
	FontDesc desc;
	desc.fullText = text;
	desc.posX = x;
	desc.posY = y;
	desc.fontSize = size;
	desc.color = color;
	desc.useFadeIn = true;  // 페이드인 사용
	desc.fadeSpeed = fadeSpeed;

	// 각 글자를 알파값 0으로 초기화
	for (wchar_t ch : text)
	{
		desc.chars.push_back({ ch, 0.f });
	}

	m_FontList.push_back(desc);
	return static_cast<int>(m_FontList.size() - 1);
}


void CFontMgr::UpdateText(int id, wstring& newText)
{
	if (id >= 0 && id < m_FontList.size())
		m_FontList[id].fullText = newText;
}

void CFontMgr::UpdatePauseText(int id, wstring& newText)
{
	if (id >= 0 && id < m_PasueFontList.size())
		m_PasueFontList[id].fullText = newText;
}

void CFontMgr::UpdatePosition(int id, float x, float y)
{
	if (id >= 0 && id < m_FontList.size())
	{
		m_FontList[id].posX = x;
		m_FontList[id].posY = y;
	}
}

void CFontMgr::UpdateTestPosition(int id, float x, float y, float z, bool _IsPause)
{
	if (_IsPause)
	{
		if (id >= 0 && id < m_PasueFontList.size())
		{
			m_PasueFontList[id].posX = x;
			m_PasueFontList[id].posY = y;
			m_PasueFontList[id].fontSize = z;
		}
	}
	else
	{
		if (id >= 0 && id < m_FontList.size())
		{
			m_FontList[id].posX = x;
			m_FontList[id].posY = y;
			m_FontList[id].fontSize = z;
		}
	}

}

void CFontMgr::UpdateColor(int id, UINT color, bool _IsPauseFont)
{
	if (_IsPauseFont)
	{
		if (id >= 0 && id < m_PasueFontList.size())
			m_PasueFontList[id].color = color;
	}
	else
	{
		if (id >= 0 && id < m_FontList.size())
			m_FontList[id].color = color;
	}

}

void CFontMgr::SetActive(int id, bool active)
{
	if (id >= 0 && id < m_FontList.size())
	{
		auto& font = m_FontList[id];
		if (!font.useFadeIn)  // 일반 텍스트인 경우만
		{
			font.isActive = active;
		}
	}
}

void CFontMgr::SetPauseActive(int id, bool active)
{
	if (id >= 0 && id < m_PasueFontList.size())
	{
		auto& font = m_PasueFontList[id];
		if (!font.useFadeIn)  // 일반 텍스트인 경우만
		{
			font.isActive = active;
		}
	}
}


void CFontMgr::SetFadeActive(int id, bool active)
{
	if (id >= 0 && id < m_FontList.size())
	{
		auto& font = m_FontList[id];
		if (font.useFadeIn)
		{
			font.isActive = active;

			if (!active)  // false일 때 초기화
			{
				font.currentCharIndex = 0;
				font.isComplete = false;
				for (auto& [ch, alpha] : font.chars)
				{
					alpha = 0.f;
				}
			}
		}
	}
}

void CFontMgr::StartFadeOut(int id, float fadeSpeed)
{
	if (id >= 0 && id < m_FontList.size())
	{
		auto& font = m_FontList[id];
		font.useFadeOut = true;
		font.isFadingOut = true;
		font.fadeSpeed = fadeSpeed;
		font.currentAlpha = 1.0f;
	}
}

bool CFontMgr::IsFadeOutComplete(int id)
{
	if (id >= 0 && id < m_FontList.size())
	{
		return m_FontList[id].currentAlpha <= 0.f;
	}
	return false;
}
int CFontMgr::RegisterSimultaneousFadeText(wstring& text, float x, float y, float size, UINT color, float fadeSpeed = 2.0f)
{
	FontDesc desc;
	desc.fullText = text;
	desc.posX = x;
	desc.posY = y;
	desc.fontSize = size;
	desc.color = color;
	desc.useSimultaneousFadeIn = true;  // 동시 페이드인 사용
	desc.fadeSpeed = fadeSpeed;
	desc.simultaneousAlpha = 0.0f;      // 초기 알파값 0

	m_FontList.push_back(desc);
	return static_cast<int>(m_FontList.size() - 1);
}

void CFontMgr::Render()
{
	if (!m_PauseCheck)
	{
		for (auto& font : m_FontList)
		{
			if (!font.isActive)
				continue;

			if (font.useFadeIn)
			{
				if (!font.isComplete && !font.chars.empty())
				{
					// 현재 활성화된 모든 글자의 알파값 업데이트
					for (size_t i = 0; i <= font.currentCharIndex; ++i)
					{
						auto& [ch, alpha] = font.chars[i];
						alpha = min(1.f, alpha + DT * font.fadeSpeed);
					}
					// 현재 글자가 0.5 이상이면 다음 글자 활성화
					auto& [ch, alpha] = font.chars[font.currentCharIndex];
					if (alpha >= 0.5f && font.currentCharIndex < font.chars.size() - 1)
					{
						font.currentCharIndex++;
					}
					// 마지막 글자의 알파값이 1.0이면 완료 처리
					if (font.currentCharIndex == font.chars.size() - 1)
					{
						const auto& [lastCh, lastAlpha] = font.chars.back();
						if (lastAlpha >= 1.0f)
						{
							font.isComplete = true;
						}
					}
				}
				// 렌더링
				float xOffset = 0.f;
				BYTE r = (font.color & 0xFF);
				BYTE g = ((font.color >> 8) & 0xFF);
				BYTE b = ((font.color >> 16) & 0xFF);

				for (size_t i = 0; i <= font.currentCharIndex && i < font.chars.size(); ++i)
				{
					const auto& [ch, alpha] = font.chars[i];
					wstring charStr(1, ch);
					UINT charColor = FONT_RGBA(r, g, b, (BYTE)(alpha * 255));
					DrawFont(charStr.c_str(),
						font.posX + xOffset,
						font.posY,
						font.fontSize,
						charColor);
					if (ch == L' ')
						xOffset += font.fontSize * 0.5f;
					else
						xOffset += font.fontSize * 1.0f;
				}
			}
			else if (font.useFadeOut && font.isFadingOut)
			{
				// 페이드아웃 처리
				font.currentAlpha = max(0.f, font.currentAlpha - DT * font.fadeSpeed);

				// 알파값 적용
				BYTE r = (font.color & 0xFF);
				BYTE g = ((font.color >> 8) & 0xFF);
				BYTE b = ((font.color >> 16) & 0xFF);
				UINT fadeColor = FONT_RGBA(r, g, b, (BYTE)(font.currentAlpha * 255));

				DrawFont(font.fullText.c_str(),
					font.posX,
					font.posY,
					font.fontSize,
					fadeColor);

				// 완전히 투명해지면 비활성화
				if (font.currentAlpha <= 0.f)
				{
					font.isActive = false;
					font.isFadingOut = false;
				}
			}
			else if (font.useSimultaneousFadeIn)
			{
				// 전체 텍스트 동시 페이드인 처리
				if (!font.isComplete)
				{
					// 알파값을 서서히 증가
					font.simultaneousAlpha = min(1.0f, font.simultaneousAlpha + DT * font.fadeSpeed);

					// 알파값이 1.0에 도달하면 완료 처리
					if (font.simultaneousAlpha >= 1.0f)
					{
						font.isComplete = true;
					}
				}

				// 알파값 적용을 위한 색상 계산
				BYTE r = (font.color & 0xFF);
				BYTE g = ((font.color >> 8) & 0xFF);
				BYTE b = ((font.color >> 16) & 0xFF);
				UINT fadeColor = FONT_RGBA(r, g, b, (BYTE)(font.simultaneousAlpha * 255));

				// 전체 텍스트를 현재 알파값으로 렌더링
				DrawFont(font.fullText.c_str(),
					font.posX,
					font.posY,
					font.fontSize,
					fadeColor);
			}
			else
			{
				// 일반 텍스트 렌더링
				DrawFont(font.fullText.c_str(),
					font.posX,
					font.posY,
					font.fontSize,
					font.color);
			}
		}
	}
	else
	{
		// Pause 상태일 경우 이쪽 렌더링이 돈다.
		for (auto& font : m_PasueFontList)
		{
			if (!font.isActive)
				continue;
            // 일반 텍스트 렌더링
            DrawFont(font.fullText.c_str(),
                font.posX,
                font.posY,
                font.fontSize,
                font.color);
		}
	}

	
}

void CFontMgr::SetSimultaneousFadeActive(int id, bool active)
{
	if (id >= 0 && id < m_FontList.size())
	{
		auto& font = m_FontList[id];
		if (font.useSimultaneousFadeIn)
		{
			font.isActive = active;

			if (!active)  // 비활성화할 때는 초기화
			{
				font.simultaneousAlpha = 0.0f;
				font.isComplete = false;
			}
		}
	}
}