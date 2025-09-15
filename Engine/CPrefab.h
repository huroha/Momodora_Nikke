#pragma once
#include "CAsset.h"


class CGameObject;



class CPrefab :
    public CAsset
{
public:
    static GAMEOBJECT_SAVE g_ObjectSave;
    static GAMEOBJECT_LOAD g_ObjectLoad;

private:
    CGameObject* m_ProtoObj;
    map<wstring, int> m_LayerIndices;       // 프리팹 각 객체의 레이어 인덱스 저장

public:
    void SetProtoObject(CGameObject* _Object);
    CGameObject* Instantiate();


    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _FilePath) override;

private:
    void SaveLayerIndices(CGameObject* _Object);
    void LoadLayerIndices(CGameObject* _Object);

public:
    CLONE(CPrefab);
    CPrefab();
    CPrefab(const CPrefab& _Origin);
    ~CPrefab();
};