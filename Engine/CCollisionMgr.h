#pragma once


union COLLIDER_ID
{
    struct
    {
        UINT Left;
        UINT Right;
    };

    ULONGLONG ID;
};




class CCollider2D;

class CCollisionMgr :
    public singleton<CCollisionMgr>
{
    SINGLE(CCollisionMgr)
private:
    UINT                    m_Matrix[MAX_LAYER];
    map<ULONGLONG, bool>    m_ColInfo;


public:
    // LevelMgr에서 Left 와 Right를 충돌 체크하도록 설정
    void CollisionCheck(UINT _Left, UINT _Right);

    // 전 레이어 충돌 체크 일괄 해제
    void CollisionCheckClear() { memset(m_Matrix, 0, sizeof(UINT) * MAX_LAYER); }

private:

    // 들어온 left와 right Layer가 충돌처리를 수행하여야 함.
    void CollisionBtwLayer(UINT _Left, UINT _Right);

    // 레이어 안의 오브젝트에 Collider Component를 확인하고, 있다면 정보를 만들어서 IsCollision으로 넘김
    void CollisionBtwCollider2D(CCollider2D* _LeftCol, CCollider2D* _RightCol);

    // 실제로 충돌한 상태인지 검사하는 단계, 회전도 체크하기 때문에 투영축 4개와 투영벡터로 계산
    bool IsCollision(CCollider2D* _Left, CCollider2D* _Right);


public:
    void Tick();
};
