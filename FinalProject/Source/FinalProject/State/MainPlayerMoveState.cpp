// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerMoveState.h"

UMainPlayerMoveState::UMainPlayerMoveState()
{
	UE_LOG(LogTemp, Log, TEXT("UMainPlayerMoveState::UMainPlayerMoveState()"));
}

void UMainPlayerMoveState::Enter_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerMoveState::Enter_Implementation()");
}

void UMainPlayerMoveState::Update_Implementation(float DeltaTime)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "UMainPlayerMoveState::Update_Implementation()");

}

void UMainPlayerMoveState::Exit_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerMoveState::Exit_Implementation()");
}

