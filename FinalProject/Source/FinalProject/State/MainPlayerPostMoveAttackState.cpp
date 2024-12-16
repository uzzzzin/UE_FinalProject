// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerPostMoveAttackState.h"
#include "../Object/MainPlayer.h"
#include "../Component/StateMachineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "../FSM/MainPlayerFSM.h"

UMainPlayerPostMoveAttackState::UMainPlayerPostMoveAttackState()
	: accTime(0.f)
	, duration(0.5f)
	, Recoveryduration(1.f)
{
}

void UMainPlayerPostMoveAttackState::Enter_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerPostMoveAttackState::Enter_Implementation()");
	accTime = 0.f;

	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());
	defaultCameraRelativePos = owner->camera->GetRelativeLocation();
}

void UMainPlayerPostMoveAttackState::Update_Implementation(float DeltaTime)
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

void UMainPlayerPostMoveAttackState::Exit_Implementation()
{
}

void UMainPlayerPostMoveAttackState::CameraZoomOutWithCurve(float DeltaTime)
{
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());

	// ��� 1. �ε巯�� �𸮾� ���� � �Լ� �̿�
	//float t = FMath::Clamp(accTime / Recoveryduration, 0.0f, 1.0f);
	//owner->springArm->TargetArmLength = FMath::InterpEaseInOut(300.0f, 600.0f, t, 2.0f); // �ε巯�� � ��ȭ


	//! ��� 2. ������ ��� �̿��� �Լ��� �̿��� �����ؿ�.
	if (accTime <= Recoveryduration) // 0 ~ ���ϴ� �ܸ�ŭ �����ϱ����. -> ���ϴ� ��ŭ �ܾƿ� �Ǿ�� �ؿ�.
	{
		FVector StartCamPos = owner->camera->GetRelativeLocation();
		FVector StartControlPoint = FVector(-25, 0, 1);

		FVector EndCamPos = StartCamPos + FVector(-10, 0, 5);
		FVector EndControlPoint = FVector(0, 0, 0); // ���� ���� ����� ���ϴ� �������� �����.

		// t ���� �����Ͽ� �ε巯�� ��� ����
		float t = FMath::Clamp(accTime / Recoveryduration, 0.0f, 1.0f);
		FVector NewPos = FMath::CubicInterp(StartCamPos, StartControlPoint, EndCamPos, EndControlPoint, t); // Cubic Bezier Curve ������ ������ ���� �Լ�.

		owner->camera->SetRelativeLocation(NewPos);
	}
	else // RecoveryDuration ~ Duration ���� ��
	{
		// �ٽ� Recovery�ؼ� ���ڸ��� ���ƿ;� �ؿ�
		FVector StartCamPos = owner->camera->GetRelativeLocation();
		FVector StartControlPoint = FVector(0, 0, 0);

		FVector EndCamPos = defaultCameraRelativePos;
		FVector EndControlPoint = FVector(0, 0, 0);

		// t ���� �����Ͽ� �ε巯�� ��� ����
		float t = FMath::Clamp((accTime - Recoveryduration) / (duration - Recoveryduration), 0.0f, 0.8f); // ���� �� �������ϰ� ���ƿ��� ���� Max���� �ٿ����.
		FVector NewPos = FMath::CubicInterp(StartCamPos, StartControlPoint, EndCamPos, EndControlPoint, t);

		owner->camera->SetRelativeLocation(NewPos);
	}
}

