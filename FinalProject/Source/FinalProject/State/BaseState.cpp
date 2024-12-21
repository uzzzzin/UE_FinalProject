// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseState.h"

UBaseState::UBaseState()
{
}

void UBaseState::Enter()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UBaseState::Enter()");
}

void UBaseState::Update(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UBaseState::Update()");
}

void UBaseState::Exit()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UBaseState::Exit()");
}
