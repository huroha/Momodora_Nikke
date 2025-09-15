#include "pch.h"
#include "CDataMgr.h"
#include "CPathMgr.h"

CDataMgr::CDataMgr()
{
	// 0. Temple
	// 1. Boss_Stage
}

CDataMgr::~CDataMgr()
{

}


void CDataMgr::Init()
{
	LoadPlayerData(CPathMgr::GetInst()->GetContentPath() + L"Save\\PlayerData.data");
	LoadStageData(CPathMgr::GetInst()->GetContentPath() + L"Save\\StageData.data");


}

int CDataMgr::SavePlayerData(const wstring& _FilePath)
{
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");
	assert(pFile);

	fwrite(&m_PlayerData.Cur_Hp, sizeof(int), 1, pFile);
	fwrite(&m_PlayerData.Max_Hp, sizeof(int), 1, pFile);
	fwrite(&m_PlayerData.Cur_Mp, sizeof(int), 1, pFile);
	fwrite(&m_PlayerData.Max_Mp, sizeof(int), 1, pFile);
	fwrite(&m_PlayerData.Atk, sizeof(int), 1, pFile);
	fwrite(&m_PlayerData.SlayCount, sizeof(int), 1, pFile);


	fclose(pFile);

	return S_OK;
}

int CDataMgr::LoadPlayerData(const wstring& _FilePath)
{
	FILE* pFile = nullptr;


	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");
	assert(pFile);

	fread(&m_PlayerData.Cur_Hp, sizeof(int), 1, pFile);
	fread(&m_PlayerData.Max_Hp, sizeof(int), 1, pFile);
	fread(&m_PlayerData.Cur_Mp, sizeof(int), 1, pFile);
	fread(&m_PlayerData.Max_Mp, sizeof(int), 1, pFile);
	fread(&m_PlayerData.Atk, sizeof(int), 1, pFile);
	fread(&m_PlayerData.SlayCount, sizeof(int), 1, pFile);


	fclose(pFile);

    return S_OK;
}

int CDataMgr::SaveStageData(const wstring& _FilePath)
{
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");
	assert(pFile);

	int VecSize = m_vecStage.size();
	fwrite(&VecSize, sizeof(int), 1, pFile);

	for (int i = 0; i < m_vecStage.size(); ++i)
	{
		fwrite(&m_vecStage[i].Idx, sizeof(int), 1, pFile);
		SaveWString(m_vecStage[i].StageName, pFile);
		SaveWString(m_vecStage[i].StageBGM, pFile);

	}


	fclose(pFile);

	return S_OK;
}

int CDataMgr::LoadStageData(const wstring& _FilePath)
{
	FILE* pFile = nullptr;


	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");
	assert(pFile);


	int Size = 0;
	fread(&Size, sizeof(int), 1, pFile);


	for (int i = 0; i < Size; ++i)
	{
		tStageData t1;
		fread(&t1.Idx, sizeof(int), 1, pFile);
		
		wstring StageName;
		LoadWString(StageName, pFile);

		wstring StageBGM;
		LoadWString(StageBGM, pFile);


		t1.StageName = StageName;
		t1.StageBGM = StageBGM;



		m_vecStage.push_back(t1);
	}


	fclose(pFile);

	return S_OK;
}

void CDataMgr::SetStageData()
{
	m_vecStage.clear();
	tStageData t1;

	// Temple 정보
	t1.Idx = 0;
	t1.StageName = L"Temple.lv";
	t1.StageBGM = L"Temple_Bgm";
	m_vecStage.push_back(t1);

	// BossStage
	t1.Idx = 1;
	t1.StageName = L"Stage_Boss.lv";
	t1.StageBGM = L"Boss_Bgm";
	m_vecStage.push_back(t1);

	// Town1Left
	t1.Idx = 2;
	t1.StageName = L"Town1L.lv";
	t1.StageBGM = L"Temple_Bgm";
	m_vecStage.push_back(t1);

	// Town1Right
	t1.Idx = 3;
	t1.StageName = L"Town1R.lv";
	t1.StageBGM = L"Temple_Bgm";
	m_vecStage.push_back(t1);

	// Tem_to_TownR
	t1.Idx = 4;
	t1.StageName = L"Tem_to_TownR.lv";
	t1.StageBGM = L"Temple_Bgm";
	m_vecStage.push_back(t1);

	// Tem_to_TownL
	t1.Idx = 5;
	t1.StageName = L"Tem_to_TownL.lv";
	t1.StageBGM = L"Temple_Bgm";
	m_vecStage.push_back(t1);


	// Town_to_DungeonD
	t1.Idx = 6;
	t1.StageName = L"Town_to_Dungeon1D.lv";
	t1.StageBGM = L"Temple_Bgm";
	m_vecStage.push_back(t1);

	// Town_to_DungeonR
	t1.Idx = 7;
	t1.StageName = L"Town_to_Dungeon1R.lv";
	t1.StageBGM = L"Temple_Bgm";
	m_vecStage.push_back(t1);

	//Dungeon1R
	t1.Idx = 8;
	t1.StageName = L"Dungeon1R.lv";
	t1.StageBGM = L"Dungeon_Bgm";
	m_vecStage.push_back(t1);

	//Dungeon1L
	t1.Idx = 9;
	t1.StageName = L"Dungeon1L.lv";
	t1.StageBGM = L"Dungeon_Bgm";
	m_vecStage.push_back(t1);

	//Dungeon2R
	t1.Idx = 10;
	t1.StageName = L"Dungeon2R.lv";
	t1.StageBGM = L"Dungeon_Bgm";
	m_vecStage.push_back(t1);

	//Dungeon2L
	t1.Idx = 11;
	t1.StageName = L"Dungeon2L.lv";
	t1.StageBGM = L"Dungeon_Bgm";
	m_vecStage.push_back(t1);

	//Dungeon3R
	t1.Idx = 12;
	t1.StageName = L"Dungeon3R.lv";
	t1.StageBGM = L"Dungeon_Bgm";
	m_vecStage.push_back(t1);

	//Dungeon3L
	t1.Idx = 13;
	t1.StageName = L"Dungeon3L.lv";
	t1.StageBGM = L"Dungeon_Bgm";
	m_vecStage.push_back(t1);

}

wstring CDataMgr::GetStageName(int _idx)
{
	// 지정되지 않은 스테이지 정보 접근 방지
	if (_idx == -1 || _idx > m_vecStage.size())
		assert(nullptr);

	return m_vecStage[_idx].StageName;

}

wstring CDataMgr::GetStageBGM(int _idx)
{
	// 지정되지 않은 스테이지 정보 접근 방지
	if (_idx == -1 || _idx > m_vecStage.size())
		assert(nullptr);

	return m_vecStage[_idx].StageBGM;
}


