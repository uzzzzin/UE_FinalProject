// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerIdleState.h"
#include "MainPlayerMoveState.h"
#include "../Object/MainPlayer.h"
#include "../Component/StateMachineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Object/MainPlayerAnimInstance.h"
#include "../FSM/MainPlayerFSM.h"

UMainPlayerIdleState::UMainPlayerIdleState()
{
}

void UMainPlayerIdleState::Enter_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerIdleState::Enter_Implementation()");
}

void UMainPlayerIdleState::Update_Implementation(float DeltaTime)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerIdleState::Update_Implementation()");

	// Idle -> Move
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());
	if (0 < owner->GetVelocity().Size())
	{
		GetOwnerFSM()->ChangeState(FName("Move"));
		return;
	}
}

void UMainPlayerIdleState::Exit_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerIdleState::Exit_Implementation()");
}