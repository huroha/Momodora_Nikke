#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"

#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CRenderMgr.h"
#include "CCollisionMgr.h"
#include "CTaskMgr.h"
#include "CFontMgr.h"
#include "CSoundMgr.h"
#include "CPrefab.h"
#include "CDataMgr.h"

CEngine::CEngine()
    : m_hMainWnd(nullptr)
    , m_FMODSystem(nullptr)
{
}
 
CEngine::~CEngine()
{
    if (nullptr != m_FMODSystem)
    {
        m_FMODSystem->release();
        m_FMODSystem = nullptr;
    }

}



int CEngine::Init(HWND _hWnd, UINT _Width, UINT _Height
    , GAMEOBJECT_SAVE _SaveFunc, GAMEOBJECT_LOAD _LoadFunc, LEVEL_LOAD _LevelLoadFunc)
{
    m_hMainWnd = _hWnd;

    m_Resolution.x = (float)_Width;
    m_Resolution.y = (float)_Height;
    
    // GameObject Save Load �Լ� ������
    CPrefab::g_ObjectSave = _SaveFunc;
    CPrefab::g_ObjectLoad = _LoadFunc;

    CLevelMgr::g_LevelLoad = _LevelLoadFunc;

    RECT rt = {0, 0, (int)m_Resolution.x, (int)m_Resolution.y};

    // Menu �� �ڵ鰪�� ���ðǵ� !! ���̸� �� �������� bool ������ �� ������.
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, !!GetMenu(m_hMainWnd));
    SetWindowPos(m_hMainWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom- rt.top, 0);

    if (FAILED(CDevice::GetInst()->Init(m_hMainWnd, m_Resolution)))
    {
        return E_FAIL;
    }
    
    // FMOD �ʱ�ȭ		
    FMOD::System_Create(&m_FMODSystem);
    assert(m_FMODSystem);

    // 32�� ä�� ����
    m_FMODSystem->init(32, FMOD_DEFAULT, nullptr);



    // Mamager �ʱ�ȭ
    CPathMgr::GetInst()->Init();
    CKeyMgr::GetInst()->Init();
    CTimeMgr::GetInst()->Init();
    CAssetMgr::GetInst()->Init();
    CRenderMgr::GetInst()->Init();
    CLevelMgr::GetInst()->Init();
    CFontMgr::GetInst()->Init();
    CSoundMgr::GetInst()->Init();
    CDataMgr::GetInst()->Init();
    





    return S_OK;
    
}

void CEngine::Progress()
{
    // FMOD Tick
    m_FMODSystem->update();

    // Engine Tick
    CKeyMgr::GetInst()->Tick();
    CTimeMgr::GetInst()->Tick();
    CSoundMgr::GetInst()->Tick();
    CLevelMgr::GetInst()->Progress();
    CCollisionMgr::GetInst()->Tick();


    // Engine Render
    CRenderMgr::GetInst()->Render();
    //CTimeMgr::GetInst()->Render();      //  Delta Time �� FPS ��Ͽ�
    CFontMgr::GetInst()->Render();

    // Task
    CTaskMgr::GetInst()->Tick();
}
