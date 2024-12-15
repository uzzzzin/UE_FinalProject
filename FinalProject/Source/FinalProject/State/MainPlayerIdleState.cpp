// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerIdleState.h"
#include "MainPlayerMoveState.h"
#include "../Object/MainPlayer.h"
#include "../Component/StateMachineComponent.h"
#include "../Object/MainPlayerAnimInstance.h"
#include <FinalProject/FSM/MainPlayerFSM.h>

UMainPlayerIdleState::UMainPlayerIdleState()
{
}

void UMainPlayerIdleState::Enter_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerIdleState::Enter_Implementation()");

	// Pitch와 Yaw 값을 사용해 Blend Space 평가
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());
	UMainPlayerAnimInstance* animInst = Cast<UMainPlayerAnimInstance>(owner->GetMesh()->GetAnimInstance());
}

void UMainPlayerIdleState::Update_Implementation(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerIdleState::Update_Implementation()");
	if (false == test)
	{
		if (acc >= dura)
		{
			test = true;

			/*AnimStates["Idle"] = false;
			AnimStates["Move"] = true;*/
			GetOwnerFSM()->ChangeState(FName("Move"));
		}
		else
		{
			acc += DeltaTime;
		}

	}
}

void UMainPlayerIdleState::Exit_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerIdleState::Exit_Implementation()");
}