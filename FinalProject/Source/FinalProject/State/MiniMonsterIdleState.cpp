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
			// ChangeState
			GetOwnerFSM()->ChangeState(FName("Move"));
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

