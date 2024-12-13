// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerIdleState.h"
#include "MainPlayerMoveState.h"
#include <FinalProject/FSM/MainPlayerFSM.h>

UMainPlayerIdleState::UMainPlayerIdleState()
{
	UE_LOG(LogTemp, Log, TEXT("UMainPlayerIdleState::UMainPlayerIdleState()"));
}

void UMainPlayerIdleState::Enter_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerIdleState::Enter_Implementation()");
}

void UMainPlayerIdleState::Update_Implementation(float DeltaTime)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerIdleState::Update_Implementation()");
	
		//FName t = FName("Move");
		//Cast<UMainPlayerStateManageComponent>(GetOwnerStateManageComponent())->ChangeState<UMainPlayerMoveState>(FName("Move"));
	if (acc > duration)
	{
		test = true;
	}
	else
	{
		acc += DeltaTime;
	}

	if (true == test)
	{
		if (prev == true)
			return;
		else
		{
			FName next = FName("Move");
			Cast<UMainPlayerFSM>(GetOwnerFSM())->ChangeState<UMainPlayerMoveState>(next);
		}
	}

	prev = test;
}

void UMainPlayerIdleState::Exit_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerIdleState::Exit_Implementation()");
}