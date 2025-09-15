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
    // LevelMgr���� Left �� Right�� �浹 üũ�ϵ��� ����
    void CollisionCheck(UINT _Left, UINT _Right);

    // �� ���̾� �浹 üũ �ϰ� ����
    void CollisionCheckClear() { memset(m_Matrix, 0, sizeof(UINT) * MAX_LAYER); }

private:

    // ���� left�� right Layer�� �浹ó���� �����Ͽ��� ��.
    void CollisionBtwLayer(UINT _Left, UINT _Right);

    // ���̾� ���� ������Ʈ�� Collider Component�� Ȯ���ϰ�, �ִٸ� ������ ���� IsCollision���� �ѱ�
    void CollisionBtwCollider2D(CCollider2D* _LeftCol, CCollider2D* _RightCol);

    // ������ �浹�� �������� �˻��ϴ� �ܰ�, ȸ���� üũ�ϱ� ������ ������ 4���� �������ͷ� ���
    bool IsCollision(CCollider2D* _Left, CCollider2D* _Right);


public:
    void Tick();
};
