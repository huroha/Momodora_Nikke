#include "pch.h"
#include "CBombIdle.h"


CBombIdle::CBombIdle()
{
}

CBombIdle::~CBombIdle()
{
}

void CBombIdle::FinalTick()
{
}

void CBombIdle::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(0, 1, false);
    // ���� ��ġ�� �̵�
    GetStateMachine()->Transform()->SetRelativePos(Vec3(2.f, 8.f, -30.f));
    

}

void CBombIdle::Exit()
{
}

