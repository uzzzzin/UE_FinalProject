// Fill out your copyright notice in the Description page of Project Settings.

#include "ReMiniMonsterIdleState.h"
#include "FSM/MiniMonsterFSM.h"

void UReMiniMonsterIdleState::Enter()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMiniMonsterIdleState::Enter_Implementation()");
	acc = 0.f;
}

void UReMiniMonsterIdleState::Update(float DeltaTime)
{
	if (false == test)
	{
		if (acc >= dura)
		{
			test = true;
			// Idle -> Patrol
			// 무조건 Idle은 시작인 게 좋을 것 같아요.
			// 기본이 Patrol
			GetOwnerFSM()->ChangeState(FName("Patrol"));
		}
		else
		{
			acc += DeltaTime;
		}

	}
}

void UReMiniMonsterIdleState::Exit()
{
}

UReMiniMonsterIdleState::UReMiniMonsterIdleState()
{
}
