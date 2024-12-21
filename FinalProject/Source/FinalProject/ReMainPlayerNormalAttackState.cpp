// Fill out your copyright notice in the Description page of Project Settings.

#include "ReMainPlayerNormalAttackState.h"
#include "Object/MainPlayer.h"
#include "Component/StateMachineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Object/MainPlayerAnimInstance.h"
#include "FSM/MainPlayerFSM.h"

UReMainPlayerNormalAttackState::UReMainPlayerNormalAttackState()
{
}

void UReMainPlayerNormalAttackState::Enter()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UReMainPlayerNormalAttackState::Enter()");
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());
	owner->PlayNormalAttackMontage();
	owner->GetCharacterMovement()->DisableMovement(); // �÷��̾� �̵��� ��Ȱ��ȭ �߾��.
}

void UReMainPlayerNormalAttackState::Update(float DeltaTime)
{
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());

	if (false == owner->GetIsAttacking())
	{
		GetOwnerFSM()->ChangeState(FName("PostNormalAttac"));
	}
}

void UReMainPlayerNormalAttackState::Exit()
{
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());
	owner->GetCharacterMovement()->SetMovementMode(MOVE_Walking); // �÷��̾� �̵��� �ٽ� Ȱ��ȭ�ؿ�.
}
