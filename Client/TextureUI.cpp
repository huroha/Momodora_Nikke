#include "pch.h"
#include "TextureUI.h"

TextureUI::TextureUI()
	: AssetUI("Texture", ASSET_TYPE::TEXTURE)
{
}

TextureUI::~TextureUI()
{
}

void TextureUI::Render_Update()
{
    AssetTitle();

    Ptr<CTexture> pAsset = dynamic_cast<CTexture*>(GetAsset().Get());
    assert(pAsset.Get());

    ImGui::Text("Name");
    ImGui::SameLine(100);

    string strKey = string(pAsset->GetKey().begin(), pAsset->GetKey().end());

    ImGui::InputText("##TexName", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_ReadOnly);

    int width = pAsset->GetWidth();
    int height = pAsset->GetHeight();

    ImGui::Text("Width");
    ImGui::SameLine(100);
    ImGui::InputInt("##Width", &width, 0.f, 0.f, ImGuiInputTextFlags_ReadOnly);

    ImGui::Text("Height");
    ImGui::SameLine(100);
    ImGui::InputInt("##Height", &height, 0.f, 0.f, ImGuiInputTextFlags_ReadOnly);

    ImVec2 uv_min = ImVec2(0.0f, 0.0f);
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);
    ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.f);
    ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
    ImGui::Image(pAsset->GetSRV().Get(), ImVec2(150.f, 150.f), uv_min, uv_max, tint_col, border_col);

    ImGui::Text("픽셀 정보");

    // 픽셀 좌표 입력 필드
    static int x = 2, y = 330;
    ImGui::InputInt("X 좌표", &x);
    ImGui::InputInt("Y 좌표", &y);

    // 텍스처 크기 가져오기
    width = pAsset->GetWidth();
    height = pAsset->GetHeight();

    // 픽셀 좌표 유효성 검사
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        // 픽셀 인덱스 계산
        UINT pixelIndex = y * width + x;

        // 픽셀 데이터 가져오기
        tPixel* pixels = pAsset->GetPixels();
        tPixel pixel = pixels[pixelIndex];

        // 0~1 사이로 정규화된 RGB 값 계산
        float normalizedR = pixel.r / 255.0f;
        float normalizedG = pixel.g / 255.0f;
        float normalizedB = pixel.b / 255.0f;

        // 픽셀 정보 표시
        ImGui::Text("(%d, %d) 픽셀 정보:", x, y);
        ImGui::Text("R: %f", normalizedR);
        ImGui::Text("G: %f", normalizedG);
        ImGui::Text("B: %f", normalizedB);
        ImGui::Text("A: %f", pixel.a / 255.0f);

        // 색상 미리보기
        ImVec4 pixelColor = ImVec4(normalizedR, normalizedG, normalizedB, pixel.a / 255.0f);
        ImGui::ColorButton("픽셀 색상", pixelColor);
    }
    else
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "잘못된 픽셀 좌표");
    }
    
}