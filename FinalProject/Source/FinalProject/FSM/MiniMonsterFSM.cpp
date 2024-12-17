// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniMonsterFSM.h"

UMiniMonsterFSM::UMiniMonsterFSM()
{
	UE_LOG(LogTemp, Log, TEXT("UMainPlayerFSM::UMainPlayerFSM()"));
}

void UMiniMonsterFSM::Initialize()
{
	if (0 == GetStateKeys().Num())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "FAIL : UMiniMonsterFSM()-> State Keys Size = 0.. ");
		return;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMiniMonsterFSM()->Initialize() ");
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( Set Cur State ---> %s )"), *GetStateKeys()[0].ToString()));
		SetCurStateName(GetStateKeys()[0]);
	}
}

void UMiniMonsterFSM::FSM_Begin()
{
	Super::FSM_Begin();
}

void UMiniMonsterFSM::FSM_Tick(float DeltaTime)
{
	Super::FSM_Tick(DeltaTime);
}

void UMiniMonsterFSM::FSM_Exit()
{
	Super::FSM_Exit();
}

