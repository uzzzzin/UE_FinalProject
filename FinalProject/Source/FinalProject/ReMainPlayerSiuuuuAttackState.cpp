// Fill out your copyright notice in the Description page of Project Settings.

#include "ReMainPlayerSiuuuuAttackState.h"
#include "Object/MainPlayer.h"
#include "Component/StateMachineComponent.h"
#include "FSM/MainPlayerFSM.h"

UReMainPlayerSiuuuuAttackState::UReMainPlayerSiuuuuAttackState()
{
}
void UReMainPlayerSiuuuuAttackState::Enter()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UReMainPlayerSiuuuuAttackState::Enter()");
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
}

