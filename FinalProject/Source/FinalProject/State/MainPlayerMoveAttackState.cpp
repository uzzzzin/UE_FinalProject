// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerMoveAttackState.h"
#include "../Object/MainPlayer.h"
#include "../Component/StateMachineComponent.h"
#include "../FSM/MainPlayerFSM.h"

UMainPlayerMoveAttackState::UMainPlayerMoveAttackState()
{
	UE_LOG(LogTemp, Warning, TEXT("UMainPlayerMoveAttackState::UMainPlayerMoveAttackState()"));
}

void UMainPlayerMoveAttackState::Enter_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerMoveAttackState::Enter_Implementation()");
}

void UMainPlayerMoveAttackState::Update_Implementation(float DeltaTime)
{
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());

	if (false == owner->GetIsAttacking())
	{
		// MoveAttack -> PostMoveAttack
		GetOwnerFSM()->ChangeState(FName("PostMoveAttack"));
	}

}

void UMainPlayerMoveAttackState::Exit_Implementation()
{
}