// FlipbookEditorUI.cpp
#include "pch.h"
#include "FlipbookEditorUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CTimeMgr.h>
#include <string>  // to_wstring�� ���� �߰�

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
    // Tick������ UpdatePreview�� ȣ��
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

    // ��ü ��� ������ ���� ���
    ImVec2 contentSize = ImGui::GetContentRegionAvail();
    float leftPanelWidth = contentSize.x * 0.4f;  // ���� �г� 40%
    float rightPanelWidth = contentSize.x * 0.6f; // ���� �г� 60%

    // === ���� �г�: ���� ���� ���� ===
    ImGui::BeginGroup();
    {
        // �ø��� �̸� �Է�
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

        // ��� ��ư���� �׷����� ����
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

        // ��������Ʈ ��� ����
        float listHeight = contentSize.y * 0.4f;
        ImGui::BeginChild("Sprite List Region", ImVec2(leftPanelWidth - 16.0f, listHeight), true);
        {
            RenderSpriteList();
        }
        ImGui::EndChild();


        // ��������Ʈ �Ӽ� ����
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

        // ���� ��ư
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

    // === ���� �г�: ������ ���� ===
    ImGui::BeginGroup();
    {
        // ������ ��Ʈ�� ����
        float controlHeight = contentSize.y * 0.15f;
        ImGui::Text("Preview Pannel");
        ImGui::BeginChild("Preview Controls", ImVec2(rightPanelWidth -6.0f, controlHeight), true);
        {
            RenderPlayerControls();
        }
        ImGui::EndChild();

        ImGui::Spacing();

        // ������ ���÷��� ����
        float previewHeight = contentSize.y * 0.8f;
        ImGui::BeginChild("Preview Display", ImVec2(rightPanelWidth - 6.0f, previewHeight), true);
        {
            RenderPreviewDisplay();
        }
        ImGui::EndChild();
    }
    ImGui::EndGroup();

    ImGui::End();

    // ���� �˾� ó��
    if (m_ShowNameError)
    {
        RenderNameErrorPopup();
    }
}

void FlipbookEditorUI::ShowFlipbookSelectUI()
{
    //// ListUI Ȱ��ȭ
    //pListUI->SetActive(true);
    wstring filePath = OpenFileDialog(L"Image Files\0*.flip;*.tga\0All Files\0*.*\0");
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

    // ���� �̸� ����
    wstring fileName = relativePath;
    size_t lastSlash = fileName.find_last_of(L"/\\");
    if (lastSlash != wstring::npos)
    {
        fileName = fileName.substr(lastSlash + 1);
    }

    string ConvertString = "Flipbook\\"+ WStringToString(fileName);

    wstring sKey = StringToWString(ConvertString);
    // ���� �Ŵ������� �ش� �̸��� Flipbook ã��
    
    Ptr<CFlipbook> pFlipbook = CAssetMgr::GetInst()->Load<CFlipbook>(sKey, relativePath);
    
    if (nullptr != pFlipbook)
    {
        LoadFlipbook(pFlipbook);
    }


}



void FlipbookEditorUI::LoadFlipbook(Ptr<CFlipbook> _pFlipbook)
{
    // ���� ���� ���̴� Flipbook ����
    if (nullptr != m_EditFlipbook && m_EditFlipbook != _pFlipbook)
    {
        // �ʿ��� ��� ���� ���� Ȯ�� ���� �߰�
    }

    // ���ο� Flipbook ����
    m_EditFlipbook = _pFlipbook;

    // UI ���� �ʱ�ȭ
    // �̸� ����
    strcpy_s(m_NameBuf, WStringToString(_pFlipbook->GetName()).c_str());

    // ù ��° ��������Ʈ ����
    m_SelectedIdx = 0;

    // ��� ���� ���� �ʱ�ȭ
    m_bPlaying = false;
    m_AccTime = 0.f;

    // ������ UI ���� ������Ʈ
    if (_pFlipbook->GetMaxSprite() > 0)
    {
        // ù ��° ��������Ʈ�� �Ӽ����� ������ �ʱ�ȭ
        Ptr<CSprite> firstSprite = _pFlipbook->GetSprite(0);
        if (nullptr != firstSprite)
        {
            // ��������Ʈ �Ӽ� UI ������Ʈ
            // RenderSpriteProperties()�� ���� �����ӿ� ȣ��� �� ������Ʈ��
        }
    }
}


void FlipbookEditorUI::RenderSpriteList()
{
    if (nullptr == m_EditFlipbook)
        return;

    ImGui::Text("Sprite List");
    ImGui::BeginChild("Sprite List", ImVec2(0, 200), true);

    // �� �տ� �߰��ϴ� ��ư
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

            // ��Ŭ�� �޴�
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

    // Background�� Offset�� �ȼ� ������ ����
    Ptr<CTexture> atlas = sprite->GetAtlasTexture();
    if (nullptr != atlas)
    {
        float atlasWidth = (float)atlas->GetWidth();
        float atlasHeight = (float)atlas->GetHeight();

        // Background ũ�� ���� (UV�� �ȼ��� ��ȯ)
        Vec2 backgroundUV = sprite->GetBackgroundUV();
        float backgroundPixels[2] = {
            backgroundUV.x * atlasWidth,  // UV ��ǥ�� �ȼ��� ��ȯ
            backgroundUV.y * atlasHeight
        };

        // �ȼ� ������ ����
        if (ImGui::DragFloat2("Background Size", backgroundPixels, 1.0f, 0.f, atlasWidth))
        {
            // �ٽ� UV�� ��ȯ�ؼ� ����
            sprite->SetBackground(Vec2(backgroundPixels[0], backgroundPixels[1]));
        }

        // Offset ���� (UV�� �ȼ��� ��ȯ)
        Vec2 offsetUV = sprite->GetOffsetUV();
        float offsetPixels[2] = {
            offsetUV.x * atlasWidth,    // UV ��ǥ�� �ȼ��� ��ȯ
            offsetUV.y * atlasHeight
        };

        // �ȼ� ������ ����
        if (ImGui::InputFloat("Offset X", &offsetPixels[0], 1.0f))
        {
            // �ٽ� UV�� ��ȯ�ؼ� ����
            sprite->SetOffset(Vec2(offsetPixels[0], offsetPixels[1]));
        }
        if (ImGui::InputFloat("Offset Y", &offsetPixels[1], 1.0f))
        {
            // �ٽ� UV�� ��ȯ�ؼ� ����
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


    // ���� �ε��� ǥ��
    ImGui::Text("Frame: %d / %d",
        m_SelectedIdx + 1,
        (nullptr != m_EditFlipbook ? m_EditFlipbook->GetMaxSprite() : 0));

    ImGui::SameLine();
    ImGui::Dummy(ImVec2(20.0f, 0.0f)); // ���� �߰�

    // ���� ������ ��ư
    if (ImGui::ArrowButton("##left", ImGuiDir_Left))
    {
        if (nullptr != m_EditFlipbook && m_SelectedIdx > 0)
        {
            m_SelectedIdx--;
            m_bPlaying = false;  // ���� ���� �� ��� ����
        }
    }

    ImGui::SameLine();

    // ���/���� ��ư
    if (ImGui::Button(m_bPlaying ? "Stop" : "Play"))
    {
        m_bPlaying ? StopPreview() : PlayPreview();
    }

    ImGui::SameLine();

    // ���� ������ ��ư
    if (ImGui::ArrowButton("##right", ImGuiDir_Right))
    {
        if (nullptr != m_EditFlipbook && m_SelectedIdx < m_EditFlipbook->GetMaxSprite() - 1)
        {
            m_SelectedIdx++;
            m_bPlaying = false;  // ���� ���� �� ��� ����
        }
    }

    ImGui::SameLine();
    ImGui::Dummy(ImVec2(20.0f, 0.0f)); // ���� �߰�

    // FPS ����
    ImGui::SetNextItemWidth(100);
    ImGui::InputFloat("FPS", &m_FPS, 1.f, 1.f);



    // �ݺ� ���
    ImGui::Checkbox("Repeat", &m_Repeat);
}

void FlipbookEditorUI::ShowSpriteSelectUI()
{
    ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
    if (!pListUI)
        return;

    // ListUI �ʱ�ȭ
    pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&FlipbookEditorUI::SelectSprite);

    // ��������Ʈ ��� ��������
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
    // ���õ� ��������Ʈ ��������
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

    // InsertIndex Ȱ��
    if (m_InsertIndex < 0 || m_InsertIndex >= m_EditFlipbook->GetMaxSprite())
    {
        // ����ó�� ���� �߰�
        m_EditFlipbook->AddSprite(_pSprite);
        m_SelectedIdx = m_EditFlipbook->GetMaxSprite() - 1;
    }
    else
    {
        // ������ ��ġ�� ���� (CFlipbook�� vector ���� ���� �ʿ�)
        vector<Ptr<CSprite>>& sprites = m_EditFlipbook->GetSpriteVector();
        sprites.insert(sprites.begin() + m_InsertIndex, _pSprite);
        m_SelectedIdx = m_InsertIndex;
    }

    // ���� �۾��� ���� InsertIndex �ʱ�ȭ
    m_InsertIndex = -1;
}

void FlipbookEditorUI::RemoveSprite(int _idx)
{
    if (nullptr == m_EditFlipbook || _idx < 0 || _idx >= m_EditFlipbook->GetMaxSprite())
        return;

    // CFlipbook�� RemoveSprite �Լ� ���
    m_EditFlipbook->RemoveSprite(_idx);

    // ���� �ε��� ����
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

    // CFlipbook�� MoveSprite �Լ� ���
    m_EditFlipbook->MoveSprite(_from, _to);
    m_SelectedIdx = _to;
}

void FlipbookEditorUI::SaveFlipbook()
{
    if (nullptr == m_EditFlipbook || m_EditFlipbook->GetMaxSprite() == 0)
        return;

    // ���ο� Flipbook�� ��쿡�� �� �̸� ����
    wstring name;
    if (m_EditFlipbook->GetKey().empty())
    {
        name = GetUniqueFlipbookName();
        m_EditFlipbook->SetName(name);

        wstring sKey = L"Flipbook\\" + name + L".flip";

        // ���� �Ŵ����� ���
        CAssetMgr::GetInst()->AddAsset(sKey, m_EditFlipbook);
    }
    else
    {
        // ���� Flipbook�� ��� �̸� ������ �Ǿ����� Ȯ�� �� ���Ӱ� ����ִ´�. �߰� �ʿ�!
        wstring beforeName = m_EditFlipbook->GetName();

        name = StringToWString(string(m_NameBuf));



    }

    // TODO: ���� ���Ϸ� �����ϴ� ���� �߰� (�ʿ��� ���)

    wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + m_EditFlipbook->GetKey();
    m_EditFlipbook->Save(strFilePath);



    // UI ���� �ʱ�ȭ
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

        // to_wstring ��� ���� ���ڸ� ���ڿ��� ��ȯ
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

    m_AccTime += EngineDT;  // DT ��� DT_Engine ���
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

    // ���� ���̾�α� ����ü �ʱ�ȭ
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = 255;
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;

    // ������ �ʱ� ���丮 ����
    wstring initialDir = CPathMgr::GetInst()->GetContentPath() + L"Flipbook\\";
    ofn.lpstrInitialDir = initialDir.c_str();

    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // ���� ���̾�α� ǥ��
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

    // ��������Ʈ UV ����
    Vec2 leftTopUV = sprite->GetLeftTopUV();
    Vec2 sliceUV = sprite->GetSliceUV();
    Vec2 backgroundUV = sprite->GetBackgroundUV();
    Vec2 offsetUV = sprite->GetOffsetUV();

    // ���� ���� ���
    ImVec2 availSize = ImGui::GetContentRegionAvail();
    float PreviewSizex = sprite->GetAtlasTexture()->GetWidth() * sprite->GetBackgroundUV().x * m_PreviewZoom;
    float PreviewSizey = sprite->GetAtlasTexture()->GetHeight() * sprite->GetBackgroundUV().y * m_PreviewZoom;

    // ȭ�鿡 �°� ������ ����
    float scale = min(
        availSize.x / PreviewSizex,
        availSize.y / PreviewSizey
    );
    scale = min(scale, 1.0f); // ���� ũ�� �̻����� Ŀ���� �ʵ���

    PreviewSizex *= scale;
    PreviewSizey *= scale;

    // �߾� ������ ���� ��ġ ���
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();
    float offsetX = (availSize.x - PreviewSizex) * 0.5f;
    float offsetY = (availSize.y - PreviewSizey) * 0.5f;
    ImGui::SetCursorScreenPos(ImVec2(cursorPos.x + offsetX, cursorPos.y + offsetY));

    // UV ���
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

    // ������ �̹��� ������
    ImGui::Image(
        sprite->GetAtlasTexture()->GetSRV().Get(),
        ImVec2(PreviewSizex, PreviewSizey),
        uv_min,
        uv_max,
        ImVec4(1, 1, 1, 1),
        ImGui::GetStyleColorVec4(ImGuiCol_Border)
    );
}