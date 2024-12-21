// Fill out your copyright notice in the Description page of Project Settings.

#include "ReMainPlayerPostNormalAttacState.h"
#include "Object/MainPlayer.h"
#include "Component/StateMachineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "FSM/MainPlayerFSM.h"

UReMainPlayerPostNormalAttacState::UReMainPlayerPostNormalAttacState()
{
}

void UReMainPlayerPostNormalAttacState::Enter()
{
}

void UReMainPlayerPostNormalAttacState::Update(float DeltaTime)
{
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());
	owner->SetIsAttacking(false);

	// PostNormalAttack -> Idle
	if (0 >= owner->GetVelocity().Size())
	{
		GetOwnerFSM()->ChangeState(FName("Idle"));
		return;
	}
	else // (0 < owner->GetVelocity().Size())
	{
		// PostNormalAttack -> Move
		GetOwnerFSM()->ChangeState(FName("Move"));
		return;
	}
}

void UReMainPlayerPostNormalAttacState::Exit()
{
}
