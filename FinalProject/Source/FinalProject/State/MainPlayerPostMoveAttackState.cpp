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

	if (accTime > duration) // Delay 시간이 끝나면
	{
		owner->GetCharacterMovement()->SetMovementMode(MOVE_Walking); // 플레이어 이동을 다시 활성화해요.

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
	else // 아직 현재 상태를 유지해야 해요.
	{
			accTime += DeltaTime;
			owner->GetCharacterMovement()->DisableMovement(); // 플레이어 이동을 비활성화 했어요.
			//CameraZoomOutWithCurve(DeltaTime);
	}

}

void UMainPlayerPostMoveAttackState::Exit_Implementation()
{
}

void UMainPlayerPostMoveAttackState::CameraZoomOutWithCurve(float DeltaTime)
{
	AMainPlayer* owner = Cast<AMainPlayer>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());

	// 방법 1. 부드러운 언리얼 제공 곡선 함수 이용
	//float t = FMath::Clamp(accTime / Recoveryduration, 0.0f, 1.0f);
	//owner->springArm->TargetArmLength = FMath::InterpEaseInOut(300.0f, 600.0f, t, 2.0f); // 부드러운 곡선 변화


	//! 방법 2. 베지에 곡선을 이용한 함수를 이용해 조절해요.
	if (accTime <= Recoveryduration) // 0 ~ 원하는 줌만큼 도달하기까지. -> 원하는 만큼 줌아웃 되어야 해요.
	{
		FVector StartCamPos = owner->camera->GetRelativeLocation();
		FVector StartControlPoint = FVector(-25, 0, 1);

		FVector EndCamPos = StartCamPos + FVector(-10, 0, 5);
		FVector EndControlPoint = FVector(0, 0, 0); // 끝점 보간 없어야 원하는 끝점에서 멈춰요.

		// t 값을 조정하여 부드러운 경로 생성
		float t = FMath::Clamp(accTime / Recoveryduration, 0.0f, 1.0f);
		FVector NewPos = FMath::CubicInterp(StartCamPos, StartControlPoint, EndCamPos, EndControlPoint, t); // Cubic Bezier Curve 공식을 구현함 내장 함수.

		owner->camera->SetRelativeLocation(NewPos);
	}
	else // RecoveryDuration ~ Duration 도달 전
	{
		// 다시 Recovery해서 제자리로 돌아와야 해요
		FVector StartCamPos = owner->camera->GetRelativeLocation();
		FVector StartControlPoint = FVector(0, 0, 0);

		FVector EndCamPos = defaultCameraRelativePos;
		FVector EndControlPoint = FVector(0, 0, 0);

		// t 값을 조정하여 부드러운 경로 생성
		float t = FMath::Clamp((accTime - Recoveryduration) / (duration - Recoveryduration), 0.0f, 0.8f); // 조금 더 스무스하게 돌아오기 위해 Max값을 줄였어요.
		FVector NewPos = FMath::CubicInterp(StartCamPos, StartControlPoint, EndCamPos, EndControlPoint, t);

		owner->camera->SetRelativeLocation(NewPos);
	}
}

