#include "pch.h"
#include "CBoss1None.h"
#include <Scripts/CBoss1SetTarget.h>


CBoss1None::CBoss1None()
{
}

CBoss1None::~CBoss1None()
{
}


void CBoss1None::FinalTick()
{
}

void CBoss1None::Enter()
{
    GetStateMachine()->FlipbookPlayer()->Play(0, 8, true);

}

void CBoss1None::Exit()
{
}
