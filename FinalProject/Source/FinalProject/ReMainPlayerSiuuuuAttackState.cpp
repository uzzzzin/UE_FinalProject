// Fill out your copyright notice in the Description page of Project Settings.

#include "ReMainPlayerSiuuuuAttackState.h"
#include "Object/MainPlayer.h"
#include "Component/StateMachineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FSM/MainPlayerFSM.h"

UReMainPlayerSiuuuuAttackState::UReMainPlayerSiuuuuAttackState()
{
}
void UReMainPlayerSiuuuuAttackState::Enter()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UReMainPlayerSiuuuuAttackState::Enter()");
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());
	owner->GetCharacterMovement()->DisableMovement(); // 플레이어 이동을 비활성화 했어요.
}

void UReMainPlayerSiuuuuAttackState::Update(float DeltaTime)
{
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());

	// SiuuuuAttack -> PostSiuuuuAttack
	if (false == owner->GetIsSiuuuuAttacking())
	{
		GetOwnerFSM()->ChangeState(FName("PostSiuuuAttack"));
	}
}

void UReMainPlayerSiuuuuAttackState::Exit()
{
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());
	owner->GetCharacterMovement()->SetMovementMode(MOVE_Walking); // 플레이어 이동을 다시 활성화해요.
}

