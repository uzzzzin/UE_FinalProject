// Fill out your copyright notice in the Description page of Project Settings.


#include "TestState.h"

void UTestState::Enter_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UTestState::Enter_Implementation()");
}

void UTestState::Update_Implementation(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UTestState::Update_Implementation()");

}

void UTestState::Exit_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UTestState::Exit_Implementation()");
}