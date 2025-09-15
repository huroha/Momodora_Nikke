// FlipbookEditorUI.cpp
#include "pch.h"
#include "FlipbookEditorUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CTimeMgr.h>
#include <string>  // to_wstring을 위해 추가

#include "CImGuiMgr.h"
#include "ListUI.h"

FlipbookEditorUI::FlipbookEditorUI()
    : EditorUI("Flipbook Editor")
    , m_EditFlipbook(nullptr)
    , m_SelectedIdx(-1)
    , m_bPlaying(false)
    , m_AccTime(0.f)
    , m_FPS(24.f)
    , m_Repeat(false)
    , m_PreviewZoom(1.f)
    , m_ShowNameError(false)
    , m_InsertIndex(-1)
{
    memset(m_NameBuf, 0, sizeof(m_NameBuf));
}

FlipbookEditorUI::~FlipbookEditorUI()
{
}

void FlipbookEditorUI::Tick()
{
    // Tick에서는 UpdatePreview만 호출
    if (m_bPlaying)
    {
        UpdatePreview();
    }
}

void FlipbookEditorUI::Render()
{
    if (!IsActive())
        return;

    bool bOpen = true;
    ImGui::Begin("Flipbook Editor", &bOpen);

    if (!bOpen)
    {
        SetActive(false);
        ImGui::End();
        return;
    }

    // 전체 사용 가능한 영역 계산
    ImVec2 contentSize = ImGui::GetContentRegionAvail();
    float leftPanelWidth = contentSize.x * 0.4f;  // 좌측 패널 40%
    float rightPanelWidth = contentSize.x * 0.6f; // 우측 패널 60%

    // === 좌측 패널: 에셋 편집 영역 ===
    ImGui::BeginGroup();
    {
        // 플립북 이름 입력
        ImGui::Text("Flipbook Name");
        ImGui::PushItemWidth(leftPanelWidth - 16.0f);
        if (ImGui::InputText("##Name", m_NameBuf, sizeof(m_NameBuf)))
        {
            wstring Name = StringToWString(string(m_NameBuf));
            m_EditFlipbook->SetName(Name);
        }
        ImGui::PopItemWidth();

        ImGui::Spacing();
        ImGui::Spacing();

        // 상단 버튼들을 그룹으로 묶음
        ImGui::BeginGroup();
        {
            float buttonWidth = (leftPanelWidth - 24.0f) * 0.5f;
            if (ImGui::Button("Add Sprite", ImVec2(buttonWidth, 0)))
            {
                ShowSpriteSelectUI();
            }
            ImGui::SameLine();
            if (ImGui::Button("Load Flipbook", ImVec2(buttonWidth, 0)))
            {
                ShowFlipbookSelectUI();
            }
        }
        ImGui::EndGroup();

        ImGui::Spacing();

        ImGui::Spacing();

        // 스프라이트 목록 영역
        float listHeight = contentSize.y * 0.4f;
        ImGui::BeginChild("Sprite List Region", ImVec2(leftPanelWidth - 16.0f, listHeight), true);
        {
            RenderSpriteList();
        }
        ImGui::EndChild();


        // 스프라이트 속성 영역
        float propertiesHeight = contentSize.y * 0.4f;
        ImGui::BeginChild("Properties Region", ImVec2(leftPanelWidth - 16.0f, propertiesHeight), true);
        {
            if (m_SelectedIdx >= 0 && nullptr != m_EditFlipbook)
            {
                RenderSpriteProperties();
            }
        }
        ImGui::EndChild();

        ImGui::Spacing();

        // 저장 버튼
        if (ImGui::Button("Save Flipbook", ImVec2(leftPanelWidth - 16.0f, 0)))
        {
            if (strlen(m_NameBuf) > 0)
                SaveFlipbook();
            else
                m_ShowNameError = true;
        }
    }
    ImGui::EndGroup();

    ImGui::SameLine();
    //ImGui::Separator();
    ImGui::SameLine();

    // === 우측 패널: 프리뷰 영역 ===
    ImGui::BeginGroup();
    {
        // 프리뷰 컨트롤 영역
        float controlHeight = contentSize.y * 0.15f;
        ImGui::Text("Preview Pannel");
        ImGui::BeginChild("Preview Controls", ImVec2(rightPanelWidth -6.0f, controlHeight), true);
        {
            RenderPlayerControls();
        }
        ImGui::EndChild();

        ImGui::Spacing();

        // 프리뷰 디스플레이 영역
        float previewHeight = contentSize.y * 0.8f;
        ImGui::BeginChild("Preview Display", ImVec2(rightPanelWidth - 6.0f, previewHeight), true);
        {
            RenderPreviewDisplay();
        }
        ImGui::EndChild();
    }
    ImGui::EndGroup();

    ImGui::End();

    // 에러 팝업 처리
    if (m_ShowNameError)
    {
        RenderNameErrorPopup();
    }
}

void FlipbookEditorUI::ShowFlipbookSelectUI()
{
    //// ListUI 활성화
    //pListUI->SetActive(true);
    wstring filePath = OpenFileDialog(L"Image Files\0*.flip;*.tga\0All Files\0*.*\0");
    if (filePath.empty())
        return;

    // Content 폴더의 전체 경로
    wstring contentPath = CPathMgr::GetInst()->GetContentPath();

    // 파일이 Content 경로 내에 있는지 확인
    size_t contentPos = filePath.find(contentPath);
    if (contentPos == wstring::npos)
        return;

    // 상대 경로 추출
    wstring relativePath = filePath.substr(contentPos + contentPath.length());

    // 파일 이름 추출
    wstring fileName = relativePath;
    size_t lastSlash = fileName.find_last_of(L"/\\");
    if (lastSlash != wstring::npos)
    {
        fileName = fileName.substr(lastSlash + 1);
    }

    string ConvertString = "Flipbook\\"+ WStringToString(fileName);

    wstring sKey = StringToWString(ConvertString);
    // 에셋 매니저에서 해당 이름의 Flipbook 찾기
    
    Ptr<CFlipbook> pFlipbook = CAssetMgr::GetInst()->Load<CFlipbook>(sKey, relativePath);
    
    if (nullptr != pFlipbook)
    {
        LoadFlipbook(pFlipbook);
    }


}



void FlipbookEditorUI::LoadFlipbook(Ptr<CFlipbook> _pFlipbook)
{
    // 현재 편집 중이던 Flipbook 정리
    if (nullptr != m_EditFlipbook && m_EditFlipbook != _pFlipbook)
    {
        // 필요한 경우 저장 여부 확인 로직 추가
    }

    // 새로운 Flipbook 설정
    m_EditFlipbook = _pFlipbook;

    // UI 상태 초기화
    // 이름 설정
    strcpy_s(m_NameBuf, WStringToString(_pFlipbook->GetName()).c_str());

    // 첫 번째 스프라이트 선택
    m_SelectedIdx = 0;

    // 재생 관련 상태 초기화
    m_bPlaying = false;
    m_AccTime = 0.f;

    // 에디터 UI 상태 업데이트
    if (_pFlipbook->GetMaxSprite() > 0)
    {
        // 첫 번째 스프라이트의 속성으로 프리뷰 초기화
        Ptr<CSprite> firstSprite = _pFlipbook->GetSprite(0);
        if (nullptr != firstSprite)
        {
            // 스프라이트 속성 UI 업데이트
            // RenderSpriteProperties()가 다음 프레임에 호출될 때 업데이트됨
        }
    }
}


void FlipbookEditorUI::RenderSpriteList()
{
    if (nullptr == m_EditFlipbook)
        return;

    ImGui::Text("Sprite List");
    ImGui::BeginChild("Sprite List", ImVec2(0, 200), true);

    // 맨 앞에 추가하는 버튼
    if (ImGui::Button("Insert At Start##sprite"))
    {
        m_InsertIndex = 0;
        ShowSpriteSelectUI();
    }

    for (int i = 0; i < m_EditFlipbook->GetMaxSprite(); i++)
    {
        Ptr<CSprite> sprite = m_EditFlipbook->GetSprite(i);
        if (nullptr != sprite)
        {
            string spriteName = WStringToString(sprite->GetName());
            string label = "[" + std::to_string(i) + "] " + spriteName;

            if (ImGui::Selectable(label.c_str(), m_SelectedIdx == i))
            {
                m_SelectedIdx = i;
            }

            // 우클릭 메뉴
            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Insert Before"))
                {
                    m_InsertIndex = i;
                    ShowSpriteSelectUI();
                }
                if (ImGui::MenuItem("Insert After"))
                {
                    m_InsertIndex = i + 1;
                    ShowSpriteSelectUI();
                }
                if (ImGui::MenuItem("Remove"))
                {
                    RemoveSprite(i);
                }
                ImGui::EndPopup();
            }
        }
    }

    ImGui::EndChild();
}

void FlipbookEditorUI::RenderSpriteProperties()
{
    if (nullptr == m_EditFlipbook || m_SelectedIdx < 0)
        return;

    ImGui::Text("Sprite Properties");
    ImGui::Separator();

    Ptr<CSprite> sprite = m_EditFlipbook->GetSprite(m_SelectedIdx);
    if (nullptr == sprite || nullptr == sprite->GetAtlasTexture())
        return;

    string spriteName = WStringToString(sprite->GetName());
    ImGui::Text("Name: %s", spriteName.c_str());

    // Background와 Offset을 픽셀 단위로 조절
    Ptr<CTexture> atlas = sprite->GetAtlasTexture();
    if (nullptr != atlas)
    {
        float atlasWidth = (float)atlas->GetWidth();
        float atlasHeight = (float)atlas->GetHeight();

        // Background 크기 조절 (UV를 픽셀로 변환)
        Vec2 backgroundUV = sprite->GetBackgroundUV();
        float backgroundPixels[2] = {
            backgroundUV.x * atlasWidth,  // UV 좌표를 픽셀로 변환
            backgroundUV.y * atlasHeight
        };

        // 픽셀 단위로 조절
        if (ImGui::DragFloat2("Background Size", backgroundPixels, 1.0f, 0.f, atlasWidth))
        {
            // 다시 UV로 변환해서 설정
            sprite->SetBackground(Vec2(backgroundPixels[0], backgroundPixels[1]));
        }

        // Offset 조절 (UV를 픽셀로 변환)
        Vec2 offsetUV = sprite->GetOffsetUV();
        float offsetPixels[2] = {
            offsetUV.x * atlasWidth,    // UV 좌표를 픽셀로 변환
            offsetUV.y * atlasHeight
        };

        // 픽셀 단위로 조절
        if (ImGui::InputFloat("Offset X", &offsetPixels[0], 1.0f))
        {
            // 다시 UV로 변환해서 설정
            sprite->SetOffset(Vec2(offsetPixels[0], offsetPixels[1]));
        }
        if (ImGui::InputFloat("Offset Y", &offsetPixels[1], 1.0f))
        {
            // 다시 UV로 변환해서 설정
            sprite->SetOffset(Vec2(offsetPixels[0], offsetPixels[1]));
        }

    }

    if (ImGui::Button("Save Sprite"))
    {
        wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + sprite->GetKey();
        sprite->Save(strFilePath);
    }
}




void FlipbookEditorUI::RenderPlayerControls()
{


    // 현재 인덱스 표시
    ImGui::Text("Frame: %d / %d",
        m_SelectedIdx + 1,
        (nullptr != m_EditFlipbook ? m_EditFlipbook->GetMaxSprite() : 0));

    ImGui::SameLine();
    ImGui::Dummy(ImVec2(20.0f, 0.0f)); // 간격 추가

    // 이전 프레임 버튼
    if (ImGui::ArrowButton("##left", ImGuiDir_Left))
    {
        if (nullptr != m_EditFlipbook && m_SelectedIdx > 0)
        {
            m_SelectedIdx--;
            m_bPlaying = false;  // 수동 변경 시 재생 중지
        }
    }

    ImGui::SameLine();

    // 재생/정지 버튼
    if (ImGui::Button(m_bPlaying ? "Stop" : "Play"))
    {
        m_bPlaying ? StopPreview() : PlayPreview();
    }

    ImGui::SameLine();

    // 다음 프레임 버튼
    if (ImGui::ArrowButton("##right", ImGuiDir_Right))
    {
        if (nullptr != m_EditFlipbook && m_SelectedIdx < m_EditFlipbook->GetMaxSprite() - 1)
        {
            m_SelectedIdx++;
            m_bPlaying = false;  // 수동 변경 시 재생 중지
        }
    }

    ImGui::SameLine();
    ImGui::Dummy(ImVec2(20.0f, 0.0f)); // 간격 추가

    // FPS 조절
    ImGui::SetNextItemWidth(100);
    ImGui::InputFloat("FPS", &m_FPS, 1.f, 1.f);



    // 반복 재생
    ImGui::Checkbox("Repeat", &m_Repeat);
}

void FlipbookEditorUI::ShowSpriteSelectUI()
{
    ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
    if (!pListUI)
        return;

    // ListUI 초기화
    pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&FlipbookEditorUI::SelectSprite);

    // 스프라이트 목록 가져오기
    const map<wstring, Ptr<CAsset>>& mapSprite = CAssetMgr::GetInst()->GetAssets(ASSET_TYPE::SPRITE);
    for (const auto& pair : mapSprite)
    {
        string strName = WStringToString(pair.second->GetKey());
        pListUI->AddItem(strName);
    }

    pListUI->SetActive(true);
}

void FlipbookEditorUI::SelectSprite(void* _ptr)
{
    ListUI* pListUI = (ListUI*)_ptr;
    string strSelectedName = pListUI->GetSelectedString();
    wstring strSpriteName = StringToWString(strSelectedName);

    //wstring sKey = L"Sprite\\" + strSpriteName + L".sprite";
    wstring sKey = strSpriteName;
    // 선택된 스프라이트 가져오기
    Ptr<CSprite> pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(sKey);
    if (nullptr != pSprite)
    {
        AddSprite(pSprite);
    }
}

void FlipbookEditorUI::AddSprite(Ptr<CSprite> _pSprite)
{
    if (nullptr == m_EditFlipbook)
    {
        m_EditFlipbook = new CFlipbook;
    }

    // InsertIndex 활용
    if (m_InsertIndex < 0 || m_InsertIndex >= m_EditFlipbook->GetMaxSprite())
    {
        // 기존처럼 끝에 추가
        m_EditFlipbook->AddSprite(_pSprite);
        m_SelectedIdx = m_EditFlipbook->GetMaxSprite() - 1;
    }
    else
    {
        // 지정된 위치에 삽입 (CFlipbook의 vector 직접 접근 필요)
        vector<Ptr<CSprite>>& sprites = m_EditFlipbook->GetSpriteVector();
        sprites.insert(sprites.begin() + m_InsertIndex, _pSprite);
        m_SelectedIdx = m_InsertIndex;
    }

    // 다음 작업을 위해 InsertIndex 초기화
    m_InsertIndex = -1;
}

void FlipbookEditorUI::RemoveSprite(int _idx)
{
    if (nullptr == m_EditFlipbook || _idx < 0 || _idx >= m_EditFlipbook->GetMaxSprite())
        return;

    // CFlipbook의 RemoveSprite 함수 사용
    m_EditFlipbook->RemoveSprite(_idx);

    // 선택 인덱스 조정
    if (m_SelectedIdx == _idx)
    {
        m_SelectedIdx = -1;
    }
    else if (m_SelectedIdx > _idx)
    {
        m_SelectedIdx--;
    }
}

void FlipbookEditorUI::MoveSprite(int _from, int _to)
{
    if (nullptr == m_EditFlipbook || _from == _to)
        return;

    if (_from < 0 || _to < 0 ||
        _from >= m_EditFlipbook->GetMaxSprite() ||
        _to >= m_EditFlipbook->GetMaxSprite())
        return;

    // CFlipbook의 MoveSprite 함수 사용
    m_EditFlipbook->MoveSprite(_from, _to);
    m_SelectedIdx = _to;
}

void FlipbookEditorUI::SaveFlipbook()
{
    if (nullptr == m_EditFlipbook || m_EditFlipbook->GetMaxSprite() == 0)
        return;

    // 새로운 Flipbook인 경우에만 새 이름 생성
    wstring name;
    if (m_EditFlipbook->GetKey().empty())
    {
        name = GetUniqueFlipbookName();
        m_EditFlipbook->SetName(name);

        wstring sKey = L"Flipbook\\" + name + L".flip";

        // 에셋 매니저에 등록
        CAssetMgr::GetInst()->AddAsset(sKey, m_EditFlipbook);
    }
    else
    {
        // 기존 Flipbook인 경우 이름 수정이 되었는지 확인 후 새롭게 집어넣는다. 추가 필요!
        wstring beforeName = m_EditFlipbook->GetName();

        name = StringToWString(string(m_NameBuf));



    }

    // TODO: 실제 파일로 저장하는 로직 추가 (필요한 경우)

    wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + m_EditFlipbook->GetKey();
    m_EditFlipbook->Save(strFilePath);



    // UI 상태 초기화
    memset(m_NameBuf, 0, sizeof(m_NameBuf));
    m_EditFlipbook = nullptr;
    m_SelectedIdx = -1;
    m_bPlaying = false;
    m_AccTime = 0.f;
}

wstring FlipbookEditorUI::GetUniqueFlipbookName() const
{
    wstring baseName = StringToWString(string(m_NameBuf));
    wstring finalName = baseName;
    int counter = 1;

    const map<wstring, Ptr<CAsset>>& mapFlipbook =
        CAssetMgr::GetInst()->GetAssets(ASSET_TYPE::FLIPBOOK);

    while (true)
    {
        bool nameExists = false;
        for (const auto& pair : mapFlipbook)
        {
            if (pair.second->GetName() == finalName)
            {
                nameExists = true;
                break;
            }
        }

        if (!nameExists)
            break;

        // to_wstring 대신 직접 숫자를 문자열로 변환
        wchar_t buf[32] = {};
        swprintf_s(buf, L" (%d)", counter++);
        finalName = baseName + buf;
    }

    return finalName;
}

void FlipbookEditorUI::RenderNameErrorPopup()
{
    ImGui::OpenPopup("Name Error##Flipbook");
    if (ImGui::BeginPopupModal("Name Error##Flipbook", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Error:");
        ImGui::Text("Please enter a name for the flipbook.");

        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            m_ShowNameError = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void FlipbookEditorUI::UpdatePreview()
{
    if (nullptr == m_EditFlipbook || !m_bPlaying || m_EditFlipbook->GetMaxSprite() == 0)
        return;

    m_AccTime += EngineDT;  // DT 대신 DT_Engine 사용
    float term = 1.f / m_FPS;

    if (term < m_AccTime)
    {
        m_AccTime -= term;
        m_SelectedIdx++;

        if (m_SelectedIdx >= m_EditFlipbook->GetMaxSprite())
        {
            if (m_Repeat)
            {
                m_SelectedIdx = 0;
            }
            else
            {
                m_SelectedIdx = m_EditFlipbook->GetMaxSprite() - 1;
                m_bPlaying = false;
            }
        }
    }
}

void FlipbookEditorUI::PlayPreview()
{
    if (nullptr == m_EditFlipbook || m_EditFlipbook->GetMaxSprite() == 0)
        return;

    m_bPlaying = true;
    m_AccTime = 0.f;
    m_SelectedIdx = 0;
}

void FlipbookEditorUI::StopPreview()
{
    m_bPlaying = false;
    m_AccTime = 0.f;
}

wstring FlipbookEditorUI::OpenFileDialog(const wchar_t* filter)
{
    OPENFILENAMEW ofn = {};
    wchar_t szFile[260] = { 0 };

    // 파일 다이얼로그 구조체 초기화
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = 255;
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;

    // 고정된 초기 디렉토리 설정
    wstring initialDir = CPathMgr::GetInst()->GetContentPath() + L"Flipbook\\";
    ofn.lpstrInitialDir = initialDir.c_str();

    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // 파일 다이얼로그 표시
    if (GetOpenFileName(&ofn))
    {
        return wstring(szFile);
    }

    return L"";
}

void FlipbookEditorUI::RenderPreviewDisplay()
{
    ImGui::Text("Preview Zoom");
    ImGui::InputFloat("##Preview Zoom", &m_PreviewZoom, 1.0f, 0.f);


    if (nullptr == m_EditFlipbook || m_SelectedIdx < 0 ||
        m_SelectedIdx >= m_EditFlipbook->GetMaxSprite())
        return;

    Ptr<CSprite> sprite = m_EditFlipbook->GetSprite(m_SelectedIdx);
    if (nullptr == sprite || nullptr == sprite->GetAtlasTexture())
        return;

    // 스프라이트 UV 정보
    Vec2 leftTopUV = sprite->GetLeftTopUV();
    Vec2 sliceUV = sprite->GetSliceUV();
    Vec2 backgroundUV = sprite->GetBackgroundUV();
    Vec2 offsetUV = sprite->GetOffsetUV();

    // 가용 영역 계산
    ImVec2 availSize = ImGui::GetContentRegionAvail();
    float PreviewSizex = sprite->GetAtlasTexture()->GetWidth() * sprite->GetBackgroundUV().x * m_PreviewZoom;
    float PreviewSizey = sprite->GetAtlasTexture()->GetHeight() * sprite->GetBackgroundUV().y * m_PreviewZoom;

    // 화면에 맞게 스케일 조정
    float scale = min(
        availSize.x / PreviewSizex,
        availSize.y / PreviewSizey
    );
    scale = min(scale, 1.0f); // 원본 크기 이상으로 커지지 않도록

    PreviewSizex *= scale;
    PreviewSizey *= scale;

    // 중앙 정렬을 위한 위치 계산
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();
    float offsetX = (availSize.x - PreviewSizex) * 0.5f;
    float offsetY = (availSize.y - PreviewSizey) * 0.5f;
    ImGui::SetCursorScreenPos(ImVec2(cursorPos.x + offsetX, cursorPos.y + offsetY));

    // UV 계산
    ImVec2 uv_min(
        leftTopUV.x + (sliceUV.x * 0.5f) - (backgroundUV.x * 0.5f),
        leftTopUV.y + (sliceUV.y * 0.5f) - (backgroundUV.y * 0.5f)
    );
    uv_min.x -= offsetUV.x;
    uv_min.y -= offsetUV.y;

    ImVec2 uv_max(
        uv_min.x + backgroundUV.x,
        uv_min.y + backgroundUV.y
    );

    // 프리뷰 이미지 렌더링
    ImGui::Image(
        sprite->GetAtlasTexture()->GetSRV().Get(),
        ImVec2(PreviewSizex, PreviewSizey),
        uv_min,
        uv_max,
        ImVec4(1, 1, 1, 1),
        ImGui::GetStyleColorVec4(ImGuiCol_Border)
    );
}