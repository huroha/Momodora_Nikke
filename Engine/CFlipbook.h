#pragma once
#include "CAsset.h"

#include "CSprite.h"
class CFlipbook :
    public CAsset
{
private:
    vector<Ptr<CSprite>>    m_vecSprite;

public:
    void AddSprite(Ptr<CSprite> _Sprite) { m_vecSprite.push_back(_Sprite); }
    UINT GetMaxSprite() { return (UINT)m_vecSprite.size(); }
    Ptr<CSprite> GetSprite(int _Idx) { return m_vecSprite[_Idx]; }

    vector<Ptr<CSprite>>& GetSpriteVector() { return m_vecSprite; }

    void RemoveSprite(int _idx)
    {
        if (_idx < 0 || _idx >= m_vecSprite.size())
            return;
        m_vecSprite.erase(m_vecSprite.begin() + _idx);
    }

    void MoveSprite(int _from, int _to)
    {
        if (_from == _to ||
            _from < 0 || _from >= m_vecSprite.size() ||
            _to < 0 || _to >= m_vecSprite.size())
            return;

        Ptr<CSprite> temp = m_vecSprite[_from];
        m_vecSprite.erase(m_vecSprite.begin() + _from);
        m_vecSprite.insert(m_vecSprite.begin() + _to, temp);
    }


private:
    virtual int Load(const wstring& _FilePath) override;

public:
    virtual int Save(const wstring& _strFilePath) override;


public:
    CLONE_DISABLE(CFlipbook);
    CFlipbook(bool _bEngineRes = false);
    ~CFlipbook();
};
