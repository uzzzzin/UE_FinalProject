// Fill out your copyright notice in the Description page of Project Settings.

#include "ReMainPlayerMoveState.h"
#include "Object/MainPlayer.h"
#include "Component/StateMachineComponent.h"
#include "FSM/MainPlayerFSM.h"

void UReMainPlayerMoveState::Enter()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UReMainPlayerMoveState::Enter()");
}

void UReMainPlayerMoveState::Update(float DeltaTime)
{
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
}

void UReMainPlayerMoveState::Exit()
{
}

UReMainPlayerMoveState::UReMainPlayerMoveState()
{
}
