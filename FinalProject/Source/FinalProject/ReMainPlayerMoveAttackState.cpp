// Fill out your copyright notice in the Description page of Project Settings.

#include "ReMainPlayerMoveAttackState.h"
#include "Object/MainPlayer.h"
#include "Component/StateMachineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Object/MainPlayerAnimInstance.h"
#include "FSM/MainPlayerFSM.h"


UReMainPlayerMoveAttackState::UReMainPlayerMoveAttackState()
{
}
void UReMainPlayerMoveAttackState::Update(float DeltaTime)
{
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());

	if (false == owner->GetIsAttacking())
	{
		// MoveAttack -> PostMoveAttack
		GetOwnerFSM()->ChangeState(FName("PostMoveAttack"));
	}
}

void UReMainPlayerMoveAttackState::Exit()
{
}

void UReMainPlayerMoveAttackState::Enter()
{
	UE_LOG(LogTemp, Warning, TEXT("UMainPlayerMoveAttackState::UMainPlayerMoveAttackState()"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerMoveAttackState::Enter_Implementation()");
}