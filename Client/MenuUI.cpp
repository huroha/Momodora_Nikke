#include "pch.h"
#include "MenuUI.h"

#include <Engine/CPathMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>

#include "CLevelSaveLoad.h"


#include "CImGuiMgr.h"
#include "Inspector.h"
#include "SpriteEditorUI.h"
#include "FlipbookEditorUI.h"

#include <Engine/CPrefab.h>
#include "ListUI.h"





MenuUI::MenuUI()
    : EditorUI("MenuUI")
{
}

MenuUI::~MenuUI()
{
}

void MenuUI::Render()
{
    if (!IsActive())
        return;

    if (ImGui::BeginMainMenuBar())
    {
        File();

        Level();

        GameObject();

        Editor();

        Asset();

        ImGui::EndMainMenuBar();
    }
}

void MenuUI::File()
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Level Save"))
        {
            CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

            // ���� ��� ���ڿ�
            wchar_t szFilePath[255] = {};

            OPENFILENAME Desc = {};

            Desc.lStructSize = sizeof(OPENFILENAME);
            Desc.hwndOwner = nullptr;
            Desc.lpstrFile = szFilePath;
            Desc.nMaxFile = 255;
            Desc.lpstrFilter = L"Level\0*.lv\0ALL\0*.*";
            Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            // ������ �ʱ� ���丮 ����
            wstring initialDir = CPathMgr::GetInst()->GetContentPath() + L"Level\\";
            Desc.lpstrInitialDir = initialDir.c_str();


            if (GetSaveFileName(&Desc))
            {
                if (wcslen(szFilePath) != 0)
                {
                    CLevelSaveLoad::SaveLevel(szFilePath, CurLevel);
                }

            }
        }

        if (ImGui::MenuItem("Level Load"))
        {
            wstring filePath = OpenFileDialog(L"Image Files\0*.lv;*.tga\0All Files\0*.*\0");

            if (!filePath.empty())
            {
                CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(filePath);
                ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

                // ������ �ε�ɶ� Inspector ���� �����ִ� ������ ���� �����Ѵ�. (������ ��ü�� ����Ű�� ���� �� �ֱ� ����)
                Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
                pInspector->SetTargetObject(nullptr);


            }


        }


        ImGui::EndMenu();
    }
}

void MenuUI::Level()
{
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    LEVEL_STATE State = LEVEL_STATE::NONE;

    if (nullptr != pCurLevel)
        State = pCurLevel->GetState();

    bool IsNotPlay = State != LEVEL_STATE::PLAY;
    bool IsPlay = !IsNotPlay;
    bool IsPause = State == LEVEL_STATE::PAUSE;

    if (ImGui::BeginMenu("Level"))
    {
        if (ImGui::MenuItem("Play", nullptr, nullptr, IsNotPlay))
        {
            //CLevelSaveLoad::SaveLevel(CPathMgr::GetInst()->GetContentPath() + L"Level\\Temp.lv", pCurLevel);

            ChangeLevelState(LEVEL_STATE::PLAY);
        }

        if (ImGui::MenuItem("Pause", nullptr, nullptr, IsPlay))
        {
            ChangeLevelState(LEVEL_STATE::PAUSE);
        }

        if (ImGui::MenuItem("Stop", nullptr, nullptr, IsPlay || IsPause))
        {
            // ������ �����ɶ� Inspector ���� �����ִ� ������ ���� �����Ѵ�. (������ ��ü�� ����Ű�� ���� �� �ֱ� ����)
            Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
            pInspector->SetTargetObject(nullptr);

            CLevel* pLevel = CLevelSaveLoad::LoadLevel(CPathMgr::GetInst()->GetContentPath() + L"Level\\Empty.lv");
            ChangeLevel(pLevel, LEVEL_STATE::STOP);
        }

        ImGui::EndMenu();
    }

}

void MenuUI::GameObject()
{
    if (ImGui::BeginMenu("GameObject"))
    {
        if (ImGui::MenuItem("Create Empty Object"))
        {
            Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
            CGameObject* pGameObj = pInspector->GetTargetObject();
            
            // ListUI �� Ȱ��ȭ ��Ű��
            ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
            pListUI->SetName("Choose Layer Idx");
            pListUI->SetActive(true);

            // ListUI �� �־��� ���ڿ� ���� ��������
            pListUI->AddItem("None");

            vector<wstring> vecStateNames;
            for (int i=0; i<MAX_LAYER; ++i)
            {
                pListUI->AddItem(layerNames[i]);
            }
            // ���� Ŭ�� �� ȣ���ų �Լ� ���
            pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&MenuUI::CreateObject_LayerIdx);
        }

        if (ImGui::MenuItem("Create PreFab"))
        {
            Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
            CGameObject* pGameObj = pInspector->GetTargetObject();

            // ListUI �� Ȱ��ȭ ��Ű��
            ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
            pListUI->SetName("Choose Layer Idx");
            pListUI->SetActive(true);

            // ListUI �� �־��� ���ڿ� ���� ��������
            pListUI->AddItem("None");

            vector<wstring> vecStateNames;
            for (int i = 0; i < MAX_LAYER; ++i)
            {
                pListUI->AddItem(layerNames[i]);
            }
            // ���� Ŭ�� �� ȣ���ų �Լ� ���
            pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&MenuUI::CreateObject_LayerIdx);
        }

        if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
        ImGui::Separator();
        if (ImGui::MenuItem("Cut", "CTRL+X")) {}
        if (ImGui::MenuItem("Copy", "CTRL+C")) {}
        if (ImGui::MenuItem("Paste", "CTRL+V")) {}
        ImGui::EndMenu();
    }
}

void MenuUI::Editor()
{
    if (ImGui::BeginMenu("Editor"))
    {
        if (ImGui::MenuItem("Sprite Editor UI"))
        {
            EditorUI* pEditor = CImGuiMgr::GetInst()->FindUI("Sprite Editor");
            pEditor->SetActive(true);
        }

        if (ImGui::MenuItem("Flipbook Editor UI"))
        {
            EditorUI* pEditor = CImGuiMgr::GetInst()->FindUI("Flipbook Editor");
            pEditor->SetActive(true);
        }

        ImGui::EndMenu();
    }
}

void MenuUI::Asset()
{
    if (ImGui::BeginMenu("Asset"))
    {
        if (ImGui::MenuItem("Create Empty Material", nullptr, nullptr))
        {
            Ptr<CMaterial> pMtrl = new CMaterial;
            wstring MtrlKey = GetNextMaterialName();
            pMtrl->SetName(L"None");
            CAssetMgr::GetInst()->AddAsset(MtrlKey, pMtrl);
        }

        if (ImGui::MenuItem("Create Prefab"))
        {
            Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
            CGameObject* pGameObj = pInspector->GetTargetObject();

            if (pGameObj != nullptr)
            {
                Ptr<CPrefab> pPrefab = new CPrefab;
                pPrefab->SetProtoObject(pGameObj);
                CAssetMgr::GetInst()->AddAsset(pGameObj->GetName(), pPrefab);
            }


        }

        ImGui::EndMenu();
    }
}


wstring MenuUI::GetNextMaterialName()
{
    wchar_t Bufff[255] = {};

    UINT i = 0;
    while (true)
    {
        wsprintf(Bufff, L"Material\\Default Material %d.mtrl", i++);

        if (nullptr == CAssetMgr::GetInst()->FindAsset<CMaterial>(Bufff))
            break;
    }

    return Bufff;
}

void MenuUI::CreateObject_LayerIdx(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
    // ����Ʈ���� ����ų���� �׸��� �̸��� �޾ƿ´�.
    ListUI* pListUI = (ListUI*)_ListUI;
    string* pStr = (string*)_SelectString;

    if (*pStr == "None")
        return;

    // �ش� Layer�̸� �� ã�Ƽ�, layerIdx�� ��ȯ�ؼ� Object ����

    wstring LayerName = wstring(pStr->begin(), pStr->end());
    string sName =WStringToString(LayerName);
    int LayerIdx = 0;
    for (int i = 0; i < MAX_LAYER; ++i)
    {
        if (sName == layerNames[i])
            LayerIdx = i;
    }
    CGameObject* pObject = nullptr;
    pObject = new CGameObject;
    pObject->SetName(L"Empty Object");

    CreateObject(pObject, LayerIdx, false);

}

void MenuUI::CreatePrefab_LayerIdx(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{

    // ����Ʈ���� ����ų���� �׸��� �̸��� �޾ƿ´�.
    ListUI* pListUI = (ListUI*)_ListUI;
    string* pStr = (string*)_SelectString;

    if (*pStr == "None")
        return;

    // �ش� Layer�̸� �� ã�Ƽ�, layerIdx�� ��ȯ�ؼ� Object ����

    wstring LayerName = wstring(pStr->begin(), pStr->end());
    string sName = WStringToString(LayerName);
    int LayerIdx = 0;
    for (int i = 0; i < MAX_LAYER; ++i)
    {
        if (sName == layerNames[i])
            LayerIdx = i;
    }
    CGameObject* pObject = nullptr;
    
    Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
    pObject = pInspector->GetTargetObject();
    
    Ptr<CPrefab> pPrefab = new CPrefab;
    pPrefab->SetProtoObject(pObject);
    CAssetMgr::GetInst()->AddAsset(pObject->GetName(), pPrefab);
}

wstring MenuUI::OpenFileDialog(const wchar_t* filter)
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
    wstring initialDir = CPathMgr::GetInst()->GetContentPath() + L"Level\\";
    ofn.lpstrInitialDir = initialDir.c_str();

    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // ���� ���̾�α� ǥ��
    if (GetOpenFileName(&ofn))
    {
        return wstring(szFile);
    }

    return L"";
}