#include "pch.h"
#include "CBombBoom.h"
#include <Engine/CSoundMgr.h>

CBombBoom::CBombBoom()
{
}

CBombBoom::~CBombBoom()
{
}

void CBombBoom::FinalTick()
{
    if (GetStateMachine()->FlipbookPlayer()->IsFinish())
    {
        GetStateMachine()->ChangeState(L"CBombIdle");
    }
}

void CBombBoom::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(1, 12, false);

    GetStateMachine()->Transform()->SetRelativeScale(Vec3(60.f, 60.f, 1.f));
    GetStateMachine()->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
    CSoundMgr::GetInst()->PlayFX(L"BombExplosion", 0.8f);

}

void CBombBoom::Exit()
{
    GetStateMachine()->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 1.f));
    GetStateMachine()->MeshRender()->SetActive(false);
}
