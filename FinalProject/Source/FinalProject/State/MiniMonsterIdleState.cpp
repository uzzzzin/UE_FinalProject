// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniMonsterIdleState.h"
#include "../FSM/MiniMonsterFSM.h"

UMiniMonsterIdleState::UMiniMonsterIdleState()
{
	UE_LOG(LogTemp, Log, TEXT("UMiniMonsterIdleState::UMiniMonsterIdleState()"));
}

void UMiniMonsterIdleState::Enter_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMiniMonsterIdleState::Enter_Implementation()");
	acc = 0.f;
}

void UMiniMonsterIdleState::Update_Implementation(float DeltaTime)
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

void UMiniMonsterIdleState::Exit_Implementation()
{
}

