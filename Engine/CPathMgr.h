#pragma once
#include "singleton.h"
class CPathMgr :
    public singleton<CPathMgr>
{
    SINGLE(CPathMgr)
private:
    wstring     m_ContentPath;
    wstring		m_BinPath;
public:
    const wstring& GetContentPath() { return m_ContentPath;}
    const wstring& GetBinPath() { return m_BinPath; }

public:
    void Init();

};

