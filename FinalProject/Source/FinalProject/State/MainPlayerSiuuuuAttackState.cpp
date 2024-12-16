// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerSiuuuuAttackState.h"
#include "../Object/MainPlayer.h"
#include "../Component/StateMachineComponent.h"
#include "../FSM/MainPlayerFSM.h"

UMainPlayerSiuuuuAttackState::UMainPlayerSiuuuuAttackState()
{
}

void UMainPlayerSiuuuuAttackState::Enter_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerSiuuuuAttackState::Enter_Implementation()");
}

void UMainPlayerSiuuuuAttackState::Update_Implementation(float DeltaTime)
{
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());

	// SiuuuuAttack -> PostSiuuuuAttack
	if (false == owner->GetIsSiuuuuAttacking())
	{
		GetOwnerFSM()->ChangeState(FName("PostSiuuuuAttack"));

	}
}

void UMainPlayerSiuuuuAttackState::Exit_Implementation()
{
}
