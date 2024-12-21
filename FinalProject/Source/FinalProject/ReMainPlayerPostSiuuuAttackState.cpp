// Fill out your copyright notice in the Description page of Project Settings.

#include "ReMainPlayerPostSiuuuAttackState.h"
#include "Object/MainPlayer.h"
#include "Component/StateMachineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Object/MainPlayerAnimInstance.h"
#include "FSM/MainPlayerFSM.h"

UReMainPlayerPostSiuuuAttackState::UReMainPlayerPostSiuuuAttackState()
	: accTime(0.f)
	, duration(0.2f)
{
}


void UReMainPlayerPostSiuuuAttackState::Enter()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UReMainPlayerPostSiuuuAttackState::Enter()");
	accTime = 0.f;
}

void UReMainPlayerPostSiuuuAttackState::Update(float DeltaTime)
{
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());

	if (accTime > duration) // Delay 시간이 끝나면
	{
		owner->GetCharacterMovement()->SetMovementMode(MOVE_Walking); // 플레이어 이동을 다시 활성화해요.

		// PostSiuuuuAttack -> Idle
		if (0 >= owner->GetVelocity().Size())
		{
			GetOwnerFSM()->ChangeState(FName("Idle"));
			return;
		}
		else // (0 < owner->GetVelocity().Size())
		{
			// PostSiuuuuAttack -> Move
			GetOwnerFSM()->ChangeState(FName("Move"));
			return;
		}
	}
	else // 아직 현재 상태를 유지해야 해요.
	{
		accTime += DeltaTime;
		owner->GetCharacterMovement()->DisableMovement(); // 플레이어 이동을 비활성화 했어요.
	}
}

void UReMainPlayerPostSiuuuAttackState::Exit()
{
}