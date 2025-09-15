#pragma once
#include <Engine/CScript.h>

struct tPlayerInfo 
{
    int     Cur_Hp;
    int     Max_Hp;
    int     Cur_Mp;
    int     Max_Mp;

    int     Atk;
    
    float   PlayerSpeed;
    int     JumpCount;
    bool    IsRollnAttack;
    bool    IsBust;

};



class CPlayerScript :
    public CScript
{
private:
    tPlayerInfo     m_Info;


    //Ptr<CTexture>   m_TargetTex;
    Ptr<CPrefab>    m_BustParticle;
    CGameObject*    m_PrefObj;
    bool            m_Create;
    float           m_SlashTime;
    float           m_SlopeAngle;

    int             m_HpFontID;
    int             m_MpFontID;



public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;


    void    CalcHp(int _Hp);         // 임시로 최대 HP 100으로만 고정
    void    CalcMp(int _Mp);         // 임시로 최대 Mp 100으로만 고정

    void    MoveLimit() { m_Info.IsRollnAttack = true; }
    // PlayerInfo 가져오기
    tPlayerInfo& GetInfo() { return m_Info; }

    void SetSlopeAngle(float _angle) { m_SlopeAngle = _angle; }
    float GetSlopeAngle() { return m_SlopeAngle; }

    // Font fade 용
    int GetFontHp_Id() { return m_HpFontID; }
    int GetFontMp_Id() { return m_MpFontID; }


public:
    CLONE(CPlayerScript);
    CPlayerScript();
    ~CPlayerScript();
};
