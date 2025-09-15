#pragma once
#include "singleton.h"

class CDataMgr :
    public singleton<CDataMgr>
{
    SINGLE(CDataMgr);

private:
    vector<tStageData> m_vecStage;
    tPlayerData        m_PlayerData;

    
public:
    void Init();
    int SavePlayerData(const wstring& _FilePath);
    int LoadPlayerData(const wstring& _FilePath);

    void SetPlayerData(int _Cur_Hp, int _Max_Hp, int _Cur_Mp, int _Max_mp, int _Atk)
    {
        m_PlayerData.Cur_Hp = _Cur_Hp;
        m_PlayerData.Max_Hp = _Max_Hp;
        m_PlayerData.Cur_Mp = _Cur_Mp;
        m_PlayerData.Max_Mp = _Max_mp;
        m_PlayerData.Atk = _Atk;
    }
    void SlayCountUp() {
        m_PlayerData.SlayCount += 1;
    }

    tPlayerData& GetPlayerData() { return m_PlayerData; }

    int SaveStageData(const wstring& _FilePath);
    int LoadStageData(const wstring& _FilePath);


    void SetStageData();

    wstring GetStageName(int _idx);
    wstring GetStageBGM(int _idx);

};

