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

	if (false == owner->GetIsQAttacking())
	{
		// MoveAttack -> PostMoveAttack
		GetOwnerFSM()->ChangeState(FName("PostMoveAttack"));
	}
}

void UReMainPlayerMoveAttackState::Exit()
{
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());

	owner->GetCharacterMovement()->SetMovementMode(MOVE_Walking); // 플레이어 이동을 다시 활성화해요.
}

void UReMainPlayerMoveAttackState::Enter()
{
	UE_LOG(LogTemp, Warning, TEXT("UMainPlayerMoveAttackState::UMainPlayerMoveAttackState()"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerMoveAttackState::Enter_Implementation()");

	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());
	owner->GetCharacterMovement()->DisableMovement(); // 플레이어 이동을 비활성화 했어요.

}