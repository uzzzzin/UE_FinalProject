// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerPostMoveAttackState.h"
#include "../Object/MainPlayer.h"
#include "../Component/StateMachineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../FSM/MainPlayerFSM.h"

UMainPlayerPostMoveAttackState::UMainPlayerPostMoveAttackState()
	: accTime(0.f)
	, duration(3.f)
{
}

void UMainPlayerPostMoveAttackState::Enter_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerPostMoveAttackState::Enter_Implementation()");
	accTime = 0.f;
}

void UMainPlayerPostMoveAttackState::Update_Implementation(float DeltaTime)
{
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());

	if (accTime > duration) // 아직 현재 상태를 유지해야 해요.
	{
		owner->GetCharacterMovement()->SetMovementMode(MOVE_Walking); // 플레이어 이동을 다시 활성화해요.

		// PostMoveAttack -> Idle
		if (0 >= owner->GetVelocity().Size())
		{
			GetOwnerFSM()->ChangeState(FName("Idle"));
			return;
		}
		else // (0 < owner->GetVelocity().Size())
		{
			// PostMoveAttack -> Move
			GetOwnerFSM()->ChangeState(FName("Move"));
			return;
		}
	}
	else // Delay 시간이 끝나면
	{
			accTime += DeltaTime;
			// TODO: 못 움직이도록.
			owner->GetCharacterMovement()->DisableMovement(); // 플레이어 이동을 비활성화 했어요.
	}

}

void UMainPlayerPostMoveAttackState::Exit_Implementation()
{
}

