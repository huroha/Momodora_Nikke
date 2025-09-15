#include "pch.h"
#include "CPlayerIdle.h"

#include <Engine/CFlipbookPlayer.h>
#include <Scripts/CPlayerScript.h>

CPlayerIdle::CPlayerIdle()
{
}

CPlayerIdle::~CPlayerIdle()
{

}

void CPlayerIdle::Enter()
{
	GetStateMachine()->FlipbookPlayer()->Play(0, 8.f, true);
}

void CPlayerIdle::FinalTick()
{

	if (GetStateMachine()->RigidBody2D()->IsGround() == false)
	{
		GetStateMachine()->ChangeState(L"CPlayerJumpDown");
	}

	if (KEY_TAP(KEY::X) && GetStateMachine()->RigidBody2D()->IsGround())
	{
		CPlayerScript* pPlayer = (CPlayerScript*)GetStateMachine()->GetOwner()->GetScripts()[0];
		float SlopeAngle = pPlayer->GetSlopeAngle();
		if (SlopeAngle == 0)
			GetStateMachine()->ChangeState(L"CPlayerRoll");
	}

	
}


void CPlayerIdle::Exit()
{


}