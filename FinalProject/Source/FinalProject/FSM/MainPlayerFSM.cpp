// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerFSM.h"

UMainPlayerFSM::UMainPlayerFSM()
{
	UE_LOG(LogTemp, Log, TEXT("UMainPlayerFSM::UMainPlayerFSM()"));
}

void UMainPlayerFSM::Initialize()
{
	if (0 == GetStateKeys().Num())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "FAIL : UMainPlayerFSM()-> State Keys Size = 0.. ");
		return;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerFSM()->Initialize() ");
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( Set Cur State ---> %s )"), *GetStateKeys()[0].ToString()));
		SetCurStateName(GetStateKeys()[0]);
	}
	//SetCurStateName(FName("Idle")); // 맨 처음은 Idle이에요.
}

void UMainPlayerFSM::FSM_Begin()
{
	Super::FSM_Begin();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerFSM::FSM_Begin()");
}

void UMainPlayerFSM::FSM_Tick(float DeltaTime)
{
	Super::FSM_Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerFSM::FSM_Tick()");
}

void UMainPlayerFSM::FSM_Exit()
{
	Super::FSM_Exit();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerFSM::FSM_Exit()");
}