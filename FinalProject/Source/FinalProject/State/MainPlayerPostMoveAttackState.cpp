// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerPostMoveAttackState.h"
#include "../Object/MainPlayer.h"
#include "../Component/StateMachineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../FSM/MainPlayerFSM.h"

UMainPlayerPostMoveAttackState::UMainPlayerPostMoveAttackState()
	: accTime(0.f)
	, duration(3.f)
{
}

void UMainPlayerPostMoveAttackState::Enter_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerPostMoveAttackState::Enter_Implementation()");
	accTime = 0.f;
}

void UMainPlayerPostMoveAttackState::Update_Implementation(float DeltaTime)
{
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());

	if (accTime > duration) // ���� ���� ���¸� �����ؾ� �ؿ�.
	{
		owner->GetCharacterMovement()->SetMovementMode(MOVE_Walking); // �÷��̾� �̵��� �ٽ� Ȱ��ȭ�ؿ�.

		// PostMoveAttack -> Idle
		if (0 >= owner->GetVelocity().Size())
		{
			GetOwnerFSM()->ChangeState(FName("Idle"));
			return;
		}
		else // (0 < owner->GetVelocity().Size())
		{
			// PostMoveAttack -> Move
			GetOwnerFSM()->ChangeState(FName("Move"));
			return;
		}
	}
	else // Delay �ð��� ������
	{
			accTime += DeltaTime;
			// TODO: �� �����̵���.
			owner->GetCharacterMovement()->DisableMovement(); // �÷��̾� �̵��� ��Ȱ��ȭ �߾��.
	}

}

void UMainPlayerPostMoveAttackState::Exit_Implementation()
{
}

