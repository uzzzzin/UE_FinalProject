// Fill out your copyright notice in the Description page of Project Settings.


#include "ReMainPlayerMoveJumpState.h"
#include "Object/MainPlayer.h"
#include "Component/StateMachineComponent.h"
#include "FSM/MainPlayerFSM.h"

UReMainPlayerMoveJumpState::UReMainPlayerMoveJumpState()
{
}

void UReMainPlayerMoveJumpState::Enter()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UReMainPlayerMoveJumpState::Enter()");

}

void UReMainPlayerMoveJumpState::Update(float DeltaTime)
{
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());

	if (false == owner->GetIsJumping()) // 점프가 끝났어요.
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

void UReMainPlayerMoveJumpState::Exit()
{
}
