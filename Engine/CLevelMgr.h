#pragma once
#include "singleton.h"

class CLevel;

class CLevelMgr :
    public singleton<CLevelMgr>
{
    SINGLE(CLevelMgr);
private:
    CLevel* m_CurLevel;



public:
    CLevel* GetCurrentLevel() { return m_CurLevel; }
    void    RegisterObject(class CGameObject* _Object);
        

    CGameObject* FindObjectByName(const wstring& _Name);
    void ChangeLevelState(LEVEL_STATE _NextState);
    void ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _NextLevelState);

    int IsPauseState();
    static LEVEL_LOAD g_LevelLoad;

public:
    void Init();
    void Progress();


    friend class CTaskMgr;
};
