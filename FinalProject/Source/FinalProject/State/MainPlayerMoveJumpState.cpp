// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerMoveJumpState.h"
#include "../Object/MainPlayer.h"
#include "../Component/StateMachineComponent.h"
#include "../FSM/MainPlayerFSM.h"

UMainPlayerMoveJumpState::UMainPlayerMoveJumpState()
{
}

void UMainPlayerMoveJumpState::Enter_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerIdleState::Enter_Implementation()");
}

void UMainPlayerMoveJumpState::Update_Implementation(float DeltaTime)
{
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());

	if (false == owner->GetIsJumping())
	{
		// MoveJump -> Idle
		if (0 >= owner->GetVelocity().Size())
		{
			GetOwnerFSM()->ChangeState(FName("Idle"));
			return;
		}
		else // (0 < owner->GetVelocity().Size())
		{
			// MoveJump -> Move
			GetOwnerFSM()->ChangeState(FName("Move"));
			return;
		}
	}
}

void UMainPlayerMoveJumpState::Exit_Implementation()
{
}

