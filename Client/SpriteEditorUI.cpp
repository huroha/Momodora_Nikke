#include "pch.h"
#include "SpriteEditorUI.h"
#include <Engine/CAssetMgr.h>

#include <commdlg.h> // ���� ���̾�α׿�

using std::to_wstring;

// ���� ��� ���� �ʱ�ȭ
char SpriteEditorUI::m_SpriteNameBuf[50] = {};

SpriteEditorUI::SpriteEditorUI()
    : EditorUI("Sprite Editor")
    , m_Atlas(nullptr)
    , m_ZoomLevel(1.0f)
    , m_PreviewSize(150, 150)
    , m_bDragging(false)
    , m_HasSelection(false)
    , m_LeftTop(0.f, 0.f)
    , m_Slice(0.f, 0.f)
    , m_Background(0.f, 0.f)
    , m_Offset(0.f, 0.f)
    , m_DragStart(0.f, 0.f)
    , m_DragCurrent(0.f, 0.f)
    , m_bPanning(false)
    , m_PanOffset(0.f, 0.f)
    , m_MouseOverAtlas(false)
    , m_ShowNameError(false)
{
}

SpriteEditorUI::~SpriteEditorUI()
{
}


void SpriteEditorUI::Render()
{
    if (!IsActive())
        return;

    // ���� ������
    string strID = GetName() + GetID();
    bool Active = IsActive();

    // �޴��� �巡�� ���� ������ ���� ���� ����
    static bool isDraggingMenuBar = false;
    static ImVec2 dragStartPos;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove;
    ImGui::Begin(strID.c_str(), &Active, window_flags);

    // X ��ư Ŭ�� ���� �� ó��
    if (Active != IsActive())
    {
        SetActive(Active);
        ImGui::End();
        return;
    }

    // �޴��� ���� ��� �� �巡�� ó��
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 mousePos = ImGui::GetMousePos();
    float menuBarHeight = ImGui::GetFrameHeight();
    bool isInMenuBar = (mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + menuBarHeight);

    if (isInMenuBar && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        isDraggingMenuBar = true;
        dragStartPos = mousePos;
    }

    if (isDraggingMenuBar && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        ImVec2 delta = ImVec2(mousePos.x - dragStartPos.x, mousePos.y - dragStartPos.y);
        ImVec2 newPos = ImVec2(windowPos.x + delta.x, windowPos.y + delta.y);
        ImGui::SetWindowPos(strID.c_str(), newPos);
        dragStartPos = mousePos;
    }

    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        isDraggingMenuBar = false;
    }

    // �޴��� ������
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Load Atlas", "Ctrl+O"))
            {
                LoadTextureFromFile();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Alt+F4"))
            {
                SetActive(false);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Reset View", "R"))
            {
                m_ZoomLevel = 1.0f;
                m_PanOffset = Vec2(0.f, 0.f);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // ���� ������ ���̾ƿ�
    ImGui::BeginChild("Atlas View", ImVec2(ImGui::GetWindowWidth() * 0.7f, 0), true,
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    RenderAtlasView();
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Right Panel", ImVec2(0, 0), true);
    RenderPreview();
    ImGui::Separator();
    RenderControlPanel();
    ImGui::EndChild();

    ImGui::End();
}

void SpriteEditorUI::RenderAtlasView()
{
    if (nullptr == m_Atlas)
    {
        ImGui::TextWrapped("No texture loaded.");

        // �ε� ��ư (�ؽ�ó�� ���� �� �߾ӿ� ũ�� ǥ��)
        ImVec2 windowSize = ImGui::GetContentRegionAvail();
        float buttonWidth = 200.0f;
        float buttonHeight = 50.0f;

        // ��ư�� �߾ӿ� ��ġ
        ImGui::SetCursorPos(ImVec2(
            (windowSize.x - buttonWidth) * 0.5f,
            (windowSize.y - buttonHeight) * 0.5f
        ));

        if (ImGui::Button("Load Atlas Texture", ImVec2(buttonWidth, buttonHeight)))
        {
            LoadTextureFromFile();
        }

        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Click to load a texture file");
        }
        return;
    }

    // �ؽ�ó�� �ε�� ����� �ڵ�
    // �� ó��
    if (ImGui::IsWindowHovered())
    {
        float wheel = ImGui::GetIO().MouseWheel;
        if (wheel != 0)
        {
            m_ZoomLevel = min(max(m_ZoomLevel + wheel * 0.1f, 0.1f), 5.0f);
        }
    }

    // �д� ó��
    HandlePanning();

    // �ؽ�ó ǥ�� ���� ���
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 availableRegion = ImGui::GetContentRegionAvail();

    // ���� �߰�
    if (ImGui::BeginChild("Toolbar", ImVec2(ImGui::GetWindowWidth(), 30), true))
    {
        if (ImGui::Button("Load New Texture"))
        {
            LoadTextureFromFile();
        }
        ImGui::SameLine();
        ImGui::Text("Zoom: %.1f%%", m_ZoomLevel * 100.0f);
        ImGui::SameLine();
        if (ImGui::Button("Reset View"))
        {
            m_ZoomLevel = 1.0f;
            m_PanOffset = Vec2(0.f, 0.f);
        }
    }
    ImGui::EndChild();

    // ���� �� ����
    float texWidth = (float)m_Atlas->GetWidth() * m_ZoomLevel;
    float texHeight = (float)m_Atlas->GetHeight() * m_ZoomLevel;

    // �д��� ����� �ؽ�ó ǥ�� ��ġ ���
    m_TextureDisplayPos = ImGui::GetCursorScreenPos();
    Vec2 adjustedPos = GetAdjustedDisplayPos();
    ImVec2 displaySize(texWidth, texHeight);

    // �̹��� ���� ���� (�д� ����)
    m_ImageMin = ImVec2(adjustedPos.x, adjustedPos.y);
    m_ImageMax = ImVec2(
        adjustedPos.x + displaySize.x,
        adjustedPos.y + displaySize.y
    );

    // �ؽ�ó ǥ�� (�д� ����)
    ImGui::SetCursorScreenPos(ImVec2(adjustedPos.x, adjustedPos.y));
    ImGui::Image(m_Atlas->GetSRV().Get(), displaySize);

    // �巡�� ó�� �� ���� ���� ǥ��
    HandleDragging();
    RenderSelectionRect();
}

void SpriteEditorUI::HandlePanning()
{
    if (ImGui::IsWindowHovered())
    {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        {
            m_bPanning = true;
            m_LastMousePos = ImGui::GetMousePos();
        }
    }

    if (m_bPanning)
    {
        ImVec2 currentMousePos = ImGui::GetMousePos();
        if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
        {
            m_PanOffset.x += currentMousePos.x - m_LastMousePos.x;
            m_PanOffset.y += currentMousePos.y - m_LastMousePos.y;
            m_LastMousePos = currentMousePos;
        }
        else
        {
            m_bPanning = false;
        }
    }
}

void SpriteEditorUI::RenderSelectionRect()
{
    if (nullptr == m_Atlas || (!m_HasSelection && !m_bDragging))
        return;

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    Vec2 displayLeftTop, displaySize;

    if (m_bDragging)
    {
        float left = min(m_DragStart.x, m_DragCurrent.x);
        float top = min(m_DragStart.y, m_DragCurrent.y);
        float width = abs(m_DragCurrent.x - m_DragStart.x);
        float height = abs(m_DragCurrent.y - m_DragStart.y);

        displayLeftTop = Vec2(left, top);
        displaySize = Vec2(width, height);
    }
    else
    {
        displayLeftTop = m_LeftTop;
        displaySize = m_Slice;
    }

    Vec2 adjustedPos = GetAdjustedDisplayPos();
    ImVec2 rectMin = ImVec2(
        adjustedPos.x + displayLeftTop.x * m_ZoomLevel,
        adjustedPos.y + displayLeftTop.y * m_ZoomLevel
    );

    ImVec2 rectMax = ImVec2(
        rectMin.x + displaySize.x * m_ZoomLevel,
        rectMin.y + displaySize.y * m_ZoomLevel
    );

    drawList->AddRect(rectMin, rectMax, IM_COL32(255, 255, 0, 255), 0.0f, 0, 1.0f);
}

void SpriteEditorUI::RenderNameErrorPopup()
{
    ImGui::OpenPopup("Name Error##Sprite");
    if (ImGui::BeginPopupModal("Name Error##Sprite", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Error:");
        ImGui::Text("Please enter a name for the sprite.");

        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            m_ShowNameError = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void SpriteEditorUI::HandleDragging()
{
    if (m_bPanning)
        return;

    // �̹��� ������ ���콺�� �ִ��� Ȯ��
    if (ImGui::IsItemHovered())
    {
        ImVec2 mousePos = ImGui::GetMousePos();
        m_DragCurrent = ScreenToTextureSpace(mousePos);

        // �巡�� ����
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !m_bDragging)
        {
            m_bDragging = true;
            m_DragStart = m_DragCurrent;

            // �巡�� ���� �� Atlas View �����쿡 ��Ŀ��
            ImGui::SetWindowFocus("Atlas View");
        }
    }

    // �巡�� ���� �� ���콺 ��ġ ������Ʈ
    if (m_bDragging)
    {
        ImVec2 mousePos = ImGui::GetMousePos();
        m_DragCurrent = ScreenToTextureSpace(mousePos);
    }

    // ���콺 ��ư�� ������ �� �巡�� ����
    if (m_bDragging && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        m_bDragging = false;
        m_HasSelection = true;
        UpdateSelectionFromDrag();
    }
}

void SpriteEditorUI::UpdateSelectionFromDrag()
{
    // �巡�� ������ �̹��� ������ ����� ��� ����
    Vec2 imageSize((float)m_Atlas->GetWidth(), (float)m_Atlas->GetHeight());

    // �������� ������ �̹��� ���� ���� ����
    Vec2 clampedStart = Vec2(
        std::clamp(m_DragStart.x, 0.f, imageSize.x),
        std::clamp(m_DragStart.y, 0.f, imageSize.y)
    );

    Vec2 clampedEnd = Vec2(
        std::clamp(m_DragCurrent.x, 0.f, imageSize.x),
        std::clamp(m_DragCurrent.y, 0.f, imageSize.y)
    );

    // ���ѵ� ��ǥ�� ���� ���� ����
    m_LeftTop = Vec2(
        min(clampedStart.x, clampedEnd.x),
        min(clampedStart.y, clampedEnd.y)
    );

    m_Slice = Vec2(
        abs(clampedEnd.x - clampedStart.x),
        abs(clampedEnd.y - clampedStart.y)
    );

    m_Background = m_Slice;
    UpdateSelectionRect();
}

void SpriteEditorUI::RenderPreview()
{
    if (nullptr == m_Atlas || (!m_HasSelection && !m_bDragging))
        return;

    ImGui::BeginChild("Preview Area", ImVec2(0, 400), true);  // ������ ���̷� Child ������ ����

    ImGui::Text("Preview");

    // ������ ������ ũ�⸦ Child �����쿡 ����
    float availWidth = ImGui::GetContentRegionAvail().x;
    float availHeight = ImGui::GetContentRegionAvail().y;

    // ���õ� ������ ���� ���
    Vec2 selectionSize = m_bDragging ?
        Vec2(abs(m_DragCurrent.x - m_DragStart.x), abs(m_DragCurrent.y - m_DragStart.y)) :
        m_Slice;

    float aspectRatio = selectionSize.x / selectionSize.y;

    // ������ ���� ������ ũ�� ����
    ImVec2 previewSize;
    if (aspectRatio > 1.0f)
    {
        previewSize.x = min(availWidth, availHeight * aspectRatio);
        previewSize.y = previewSize.x / aspectRatio;
    }
    else
    {
        previewSize.y = min(availHeight, availWidth / aspectRatio);
        previewSize.x = previewSize.y * aspectRatio;
    }

    // �߾� ������ ���� ��ġ ���
    ImVec2 startPos = ImGui::GetCursorScreenPos();
    float offsetX = (availWidth - previewSize.x) * 0.5f;
    float offsetY = (availHeight - previewSize.y) * 0.5f;

    // ��� ����
    ImGui::GetWindowDrawList()->AddRectFilled(
        startPos,
        ImVec2(startPos.x + availWidth, startPos.y + availHeight),
        IM_COL32(50, 50, 50, 255)
    );

    // �̹��� ��ġ ���� �� ������
    ImVec2 centeredPos = ImVec2(startPos.x + offsetX, startPos.y + offsetY);
    ImGui::SetCursorScreenPos(centeredPos);

    // UV ��ǥ ���
    Vec2 previewUV0, previewUV1;
    if (m_bDragging)
    {
        float left = min(m_DragStart.x, m_DragCurrent.x);
        float top = min(m_DragStart.y, m_DragCurrent.y);
        float right = max(m_DragStart.x, m_DragCurrent.x);
        float bottom = max(m_DragStart.y, m_DragCurrent.y);

        // UV ��ǥ�� �̹��� ���� ���� ����
        left = std::clamp(left, 0.f, (float)m_Atlas->GetWidth());
        top = std::clamp(top, 0.f, (float)m_Atlas->GetHeight());
        right = std::clamp(right, 0.f, (float)m_Atlas->GetWidth());
        bottom = std::clamp(bottom, 0.f, (float)m_Atlas->GetHeight());

        previewUV0 = Vec2(left / m_Atlas->GetWidth(), top / m_Atlas->GetHeight());
        previewUV1 = Vec2(right / m_Atlas->GetWidth(), bottom / m_Atlas->GetHeight());
    }
    else
    {
        previewUV0 = Vec2(
            m_LeftTop.x / m_Atlas->GetWidth(),
            m_LeftTop.y / m_Atlas->GetHeight()
        );
        previewUV1 = Vec2(
            (m_LeftTop.x + m_Slice.x) / m_Atlas->GetWidth(),
            (m_LeftTop.y + m_Slice.y) / m_Atlas->GetHeight()
        );
    }

    // �̹��� ������
    ImGui::Image(
        m_Atlas->GetSRV().Get(),
        previewSize,
        ImVec2(previewUV0.x, previewUV0.y),
        ImVec2(previewUV1.x, previewUV1.y)
    );

    ImGui::EndChild();
}

void SpriteEditorUI::RenderControlPanel()
{
    // ���� ����
    ImGui::Text("Sprite Properties");
    ImGui::Separator();

    // �ؽ�ó�� �ε���� ���� ��� ó��
    if (nullptr == m_Atlas)
    {
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "No Atlas Texture Loaded");
        return;
    }

    // =========================================
    // ��������Ʈ �̸� �Է� ����
    // =========================================
    ImGui::Text("Sprite Name:");
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Enter a unique name for the sprite");
    }

    // �̸� �Է� �ʵ� (##�� ���� �������� ���� ID�� ���)
    ImGui::InputText("##SpriteName", m_SpriteNameBuf, sizeof(m_SpriteNameBuf));

    ImGui::Separator();

    // =========================================
    // ��������Ʈ �Ӽ� ���� ����
    // =========================================



    // ���� ������ ��ư
    if (ImGui::ArrowButton("##left", ImGuiDir_Left))
    {
        if (m_LeftTop.x > 0)
            m_LeftTop.x -= m_Slice.x;
        else
        {
            m_LeftTop.x = m_Atlas->GetWidth() - m_Slice.x;
            m_LeftTop.y -= m_Slice.y;
        }
    }

    ImGui::SameLine();

    // ���� ������ ��ư
    if (ImGui::ArrowButton("##right", ImGuiDir_Right))
    {
        if (m_LeftTop.x < m_Atlas->GetWidth() - m_Slice.x)
            m_LeftTop.x += m_Slice.x;
        else
        {
            m_LeftTop.x = 0;
            m_LeftTop.y += m_Slice.y;
        }

    }


    // Left Top ��ġ ����
    float leftTop[2] = { m_LeftTop.x, m_LeftTop.y };
    if (ImGui::DragFloat2("Left Top", leftTop, 1.0f, 0.0f, (float)m_Atlas->GetWidth()))
    {
        m_LeftTop = Vec2(leftTop[0], leftTop[1]);
        UpdateSelectionRect();  // ���� ����Ǹ� ���� ���� ������Ʈ
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Left-top position in the atlas (pixels)");
    }



    // Slice(�߶� ����) ũ�� ����
    float slice[2] = { m_Slice.x, m_Slice.y };
    if (ImGui::DragFloat2("Slice Size", slice, 1.0f, 0.0f, (float)m_Atlas->GetWidth()))
    {
        m_Slice = Vec2(slice[0], slice[1]);
        UpdateSelectionRect();
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Size of the sprite selection (pixels)");
    }

    // Background(���) ũ�� ����
    float background[2] = { m_Background.x, m_Background.y };
    if (ImGui::DragFloat2("Background Size", background, 1.0f, 0.0f, (float)m_Atlas->GetWidth()))
    {
        m_Background = Vec2(background[0], background[1]);
        UpdateSelectionRect();
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Size of the sprite background (pixels)");
    }

    // Offset(������) ����
    float offset[2] = { m_Offset.x, m_Offset.y };
    if (ImGui::DragFloat2("Offset", offset, 1.0f))
    {
        m_Offset = Vec2(offset[0], offset[1]);
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Sprite offset from origin");
    }



    // =========================================
    // ���� ǥ�� ����
    // =========================================
    ImGui::Separator();
    ImGui::Text("Selection Info:");
    ImGui::Text("Position: (%.0f, %.0f)", m_LeftTop.x, m_LeftTop.y);
    ImGui::Text("Size: %.0f x %.0f", m_Slice.x, m_Slice.y);

    ImGui::Separator();

    // =========================================
    // ��������Ʈ ���� ��ư ����
    // =========================================
    // ��ư ���� ����
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.6f, 0.2f, 1.0f));

    // ���� ��ư
    if (ImGui::Button("Create Sprite", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
    {
        if (strlen(m_SpriteNameBuf) > 0)
        {
            // �Էµ� �̸����� ��������Ʈ ����
            wstring spriteName = StringToWString(string(m_SpriteNameBuf));
            CreateSprite(spriteName);

            // �Է� �ʵ� �ʱ�ȭ
            memset(m_SpriteNameBuf, 0, sizeof(m_SpriteNameBuf));
        }
        else
        {
            // �̸��� ����ִ� ��� ���� �˾� ǥ��
            m_ShowNameError = true;
        }
    }

    // ��ư ��Ÿ�� ����
    ImGui::PopStyleColor(3);

    // ��ư ���� ����
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Creates a new sprite with the current selection");
    }

    // ���� �˾� ó��
    if (m_ShowNameError)
    {
        RenderNameErrorPopup();
    }
}

void SpriteEditorUI::CreateSprite(const wstring& _Name)
{
    if (nullptr == m_Atlas || !m_HasSelection)
        return;

    // ������ �̸� ����
    wstring finalName = GetUniqueSpriteNameFromInput();

    // ��������Ʈ ���� �� ����
    Ptr<CSprite> pSprite = new CSprite;
    pSprite->SetName(finalName);
    pSprite->SetAtlasTexture(m_Atlas);
    pSprite->SetLeftTop(m_LeftTop);
    pSprite->SetSlice(m_Slice);
    pSprite->SetBackground(m_Background);
    pSprite->SetOffset(m_Offset);

    wstring sKey = L"Sprite\\" + finalName + L".sprite";


    // ���� �Ŵ����� ���
    CAssetMgr::GetInst()->AddAsset(sKey, pSprite);
    wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + pSprite->GetKey();
    pSprite->Save(strFilePath);

}

// ��ƿ��Ƽ �Լ��� ����
Vec2 SpriteEditorUI::GetAdjustedDisplayPos() const
{
    return Vec2(
        m_TextureDisplayPos.x + m_PanOffset.x,
        m_TextureDisplayPos.y + m_PanOffset.y
    );
}

Vec2 SpriteEditorUI::ScreenToTextureSpace(const ImVec2& screenPos)
{
    Vec2 adjustedPos = GetAdjustedDisplayPos();
    return Vec2(
        (screenPos.x - adjustedPos.x) / m_ZoomLevel,
        (screenPos.y - adjustedPos.y) / m_ZoomLevel
    );
}

bool SpriteEditorUI::IsInImageBounds(const ImVec2& point)
{
    return point.x >= m_ImageMin.x && point.x <= m_ImageMax.x &&
        point.y >= m_ImageMin.y && point.y <= m_ImageMax.y;
}

void SpriteEditorUI::UpdateSelectionRect()
{
    if (nullptr == m_Atlas)
        return;

    // ��ǥ�� �ؽ�ó ������ ����� �ʵ��� ����
    m_LeftTop.x = min(max(m_LeftTop.x, 0.f), (float)m_Atlas->GetWidth());
    m_LeftTop.y = min(max(m_LeftTop.y, 0.f), (float)m_Atlas->GetHeight());

    // ���� ������ �ؽ�ó�� ����� �ʵ��� ����
    m_Slice.x = min(m_Slice.x, (float)m_Atlas->GetWidth() - m_LeftTop.x);
    m_Slice.y = min(m_Slice.y, (float)m_Atlas->GetHeight() - m_LeftTop.y);

    // ��� ũ�⵵ ����
    m_Background.x = min(m_Background.x, (float)m_Atlas->GetWidth());
    m_Background.y = min(m_Background.y, (float)m_Atlas->GetHeight());
}

void SpriteEditorUI::LoadTextureFromFile()
{
    wstring filePath = OpenFileDialog(L"Image Files\0*.png;*.jpg;*.jpeg;*.bmp;*.dds;*.tga\0All Files\0*.*\0");
    if (filePath.empty())
        return;

    // Content ������ ��ü ���
    wstring contentPath = CPathMgr::GetInst()->GetContentPath();

    // ������ Content ��� ���� �ִ��� Ȯ��
    size_t contentPos = filePath.find(contentPath);
    if (contentPos == wstring::npos)
        return;

    // ��� ��� ����
    wstring relativePath = filePath.substr(contentPos + contentPath.length());

    // ���� ��� ����
    wstring fileName = relativePath;
    size_t lastSlash = fileName.find_last_of(L"/\\");
    if (lastSlash != wstring::npos)
    {
        fileName = fileName.substr(lastSlash + 1);
    }

    // �ؽ�ó �ε� �õ�
    m_Atlas = CAssetMgr::GetInst()->Load<CTexture>(fileName, relativePath);

    if (m_Atlas != nullptr)
    {
        // �ε� ���� �� �ʱ�ȭ
        m_HasSelection = false;
        m_bDragging = false;
        m_PanOffset = Vec2(0.f, 0.f);
        m_ZoomLevel = 1.0f;
    }
}

wstring SpriteEditorUI::GetUniqueSpriteNameFromInput() const
{
    // �Էµ� �̸��� �����ͼ� wstring���� ��ȯ
    wstring baseName = StringToWString(string(m_SpriteNameBuf));
    wstring finalName = baseName;
    int counter = 1;

    // �ߺ� �̸� �˻� �� ó��
    const map<wstring, Ptr<CAsset>>& mapSprite = CAssetMgr::GetInst()->GetAssets(ASSET_TYPE::SPRITE);
    while (true)
    {
        // ���� �̸��� �̹� �����ϴ��� �˻�
        bool nameExists = false;
        for (const auto& pair : mapSprite)
        {
            if (pair.second->GetName() == finalName)
            {
                nameExists = true;
                break;
            }
        }

        // �ߺ����� �ʴ� �̸��� ã�Ҵٸ� ��ȯ
        if (!nameExists)
            break;

        // �ߺ��ȴٸ� ��ȣ�� �������� �� �̸� ����
        // ��: "MySprite" -> "MySprite (1)" -> "MySprite (2)"
        finalName = baseName + L" (" + to_wstring(counter++) + L")";
    }

    return finalName;
}

wstring SpriteEditorUI::OpenFileDialog(const wchar_t* filter)
{
    OPENFILENAMEW ofn = {};
    wchar_t szFile[260] = { 0 };
    
    // ���� ���̾�α� ����ü �ʱ�ȭ
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = GetActiveWindow();
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    
    // ������ �ʱ� ���丮 ����
    wstring initialDir = CPathMgr::GetInst()->GetContentPath() + L"Texture\\";
    ofn.lpstrInitialDir = initialDir.c_str();
    
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    
    // ���� ���̾�α� ǥ��
    if (GetOpenFileNameW(&ofn) == TRUE)
    {
        return wstring(szFile);
    }
    
    return L"";
}