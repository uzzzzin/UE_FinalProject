// Fill out your copyright notice in the Description page of Project Settings.

#include "ReMainPlayerIdleState.h"
#include "Object/MainPlayer.h"
#include "Component/StateMachineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Object/MainPlayerAnimInstance.h"
#include "FSM/MainPlayerFSM.h"

UReMainPlayerIdleState::UReMainPlayerIdleState()
{
}
void UReMainPlayerIdleState::Enter()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UReMainPlayerIdleState::Enter()");
}

void UReMainPlayerIdleState::Update(float DeltaTime)
{
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());
	if (0 < owner->GetVelocity().Size())
	{
		GetOwnerFSM()->ChangeState(FName("Move"));
		return;
	}

	if (true == owner->GetIsQAttacking())
	{
		GetOwnerFSM()->ChangeState(FName("MoveAttack"));
		return;
	}

	if (true == owner->GetIsSiuuuuAttacking())
	{
		GetOwnerFSM()->ChangeState(FName("SiuuuuAttack"));
		return;
	}

	if (true == owner->GetIsAttacking())
	{
		GetOwnerFSM()->ChangeState(FName("NormalAttack"));
		return;
	}

}

void UReMainPlayerIdleState::Exit()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerIdleState::Exit_Implementation()");
}

