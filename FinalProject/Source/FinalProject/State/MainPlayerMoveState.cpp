// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerMoveState.h"
#include "../Object/MainPlayer.h"
#include "../Component/StateMachineComponent.h"
#include "../FSM/MainPlayerFSM.h"

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

	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());
	// Move -> Idle
	if (0 >= owner->GetVelocity().Size())
	{
		GetOwnerFSM()->ChangeState(FName("Idle"));
		return;
	}

	// Move -> MoveJump
	if (true == owner->GetIsJumping())
	{
		GetOwnerFSM()->ChangeState(FName("MoveJump"));
		return;
	}

	// Move -> MoveAttack
	if (true == owner->GetIsAttacking())
	{
		GetOwnerFSM()->ChangeState(FName("MoveAttack"));
		return;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "????????????????????");
	}
}

void UMainPlayerMoveState::Exit_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerMoveState::Exit_Implementation()");
}

