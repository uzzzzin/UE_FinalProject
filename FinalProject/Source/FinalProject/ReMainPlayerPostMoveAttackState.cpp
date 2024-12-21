// Fill out your copyright notice in the Description page of Project Settings.

#include "ReMainPlayerPostMoveAttackState.h"
#include "Object/MainPlayer.h"
#include "Component/StateMachineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "FSM/MainPlayerFSM.h"

UReMainPlayerPostMoveAttackState::UReMainPlayerPostMoveAttackState()
	: accTime(0.f)
	, duration(0.5f)
	, Recoveryduration(1.f)
{
}
void UReMainPlayerPostMoveAttackState::Enter()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerPostMoveAttackState::Enter_Implementation()");
	accTime = 0.f;

	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());
	defaultCameraRelativePos = owner->camera->GetRelativeLocation();
}

void UReMainPlayerPostMoveAttackState::Update(float DeltaTime)
{
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());

	if (accTime > duration) // Delay �ð��� ������
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
	else // ���� ���� ���¸� �����ؾ� �ؿ�.
	{
		accTime += DeltaTime;
		owner->GetCharacterMovement()->DisableMovement(); // �÷��̾� �̵��� ��Ȱ��ȭ �߾��.
		//CameraZoomOutWithCurve(DeltaTime);
	}
}

void UReMainPlayerPostMoveAttackState::Exit()
{
}

