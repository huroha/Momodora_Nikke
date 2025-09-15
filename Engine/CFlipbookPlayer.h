#pragma once
#include "CComponent.h"

#include "CFlipbook.h"

class CFlipbookPlayer :
    public CComponent
{
private:
    vector<Ptr<CFlipbook>>  m_vecFlipbook;      // ��� ��� Flipbook
    Ptr<CFlipbook>          m_CurFlipbook;      // ������� Flipbook
    int                     m_SpriteIdx;        // ������� Sprite �ε���
    bool                    m_Repeat;           // �ݺ����
    float                   m_FPS;              // ����ӵ�
    float                   m_Time;             // ���� �ð�
    bool                    m_Finish;           // ��� ȸ���� ������ ��
    int                     m_FlipbookCount;    // ���� �������ִ� flipbook ����

    bool                    m_IsHit;
    float                   m_HitEffectTime;
    float                   m_HitDuration;
    Vec3                    m_HitColor;
public:
    void AddFlipbook(int _idx, Ptr<CFlipbook> _Flipbook);
    void Play(int _Idx, float _FPS, bool _Repeat)
    {
        m_CurFlipbook = m_vecFlipbook[_Idx];
        m_FPS = _FPS;
        m_Repeat = _Repeat;
        m_Time = 0.f;
        m_SpriteIdx = 0;
        m_Finish = false;

    }

    vector<Ptr<CFlipbook>>& GetSpriteVector() { return m_vecFlipbook; }
    Ptr<CFlipbook>& GetCurFlipbook() { return m_CurFlipbook; }
    Ptr<CSprite> GetCurrentSprite() { return m_CurFlipbook->GetSprite(m_SpriteIdx); }
    bool    GetRepeat() { return m_Repeat; }
    float   GetFPS() { return m_FPS; }
    int     GetFlipbookSize() { return m_vecFlipbook.size(); }
    int     GetSpriteIdx() { return m_SpriteIdx; }

    void    RemoveFlipbook();
    int     GetFlipbookCount() { return m_FlipbookCount; }

    void SetRepeat(bool _Repeat) { m_Repeat = _Repeat; }
    void SetFPS(float _FPS) { m_FPS = _FPS; }
    bool    IsFinish() { return m_Finish; }


    void Binding();
    void Clear();

    // ��Ʈ ȿ�� ���� ��� (0~1)
    float CalculateHitRatio()
    {
        if (!m_IsHit)
            return 0.f;

        float hitProgress = m_HitEffectTime / m_HitDuration;
        return sin(hitProgress * 15.0f) * 0.5f + 0.5f;
    }


    void StartHitEffect(float _duration, Vec3 hitColor);


    void UpdateHitEffect();

public:
    virtual void FinalTick() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

public:
    CLONE(CFlipbookPlayer);
    CFlipbookPlayer();
    ~CFlipbookPlayer();
};