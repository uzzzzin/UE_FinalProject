// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerAnimInstance.h"
#include "MainPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

UMainPlayerAnimInstance::UMainPlayerAnimInstance()
	: bIsMoving(false)
	, bIsAttacking(false)
	, bAttackEnded(true) // �ʱ�� �ٷ� ���� ������ �� �ֵ��� true.
{
}

void UMainPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMainPlayer* Owner = Cast<AMainPlayer>(TryGetPawnOwner());
	if (Owner)
	{
		//! BP-> Anim State ����
		bIsAttacking = Owner->GetIsAttacking();
		bIsMoving = Owner->GetIsMoving();
		bIsJumping = Owner->GetIsJumping();

		if (false == bIsMoving) // State : Move -> Idle �ʱ� ���� ����
		{
			InputMoveHorizontal = 0.f;
			InputMoveVertical = 0.f;
		}
		else // true == bIsMoving // State : Idle -> Move �ʱ� ���� ����
		{
			InputMouseYaw = 0.f;
			InputMousePitch = 0.f;
		}
	}
}

void UMainPlayerAnimInstance::AddMouseYaw(float _v)
{
	if (0 != _v)
	{
		InputMouseYaw += _v; // Input���� �������Ѽ� ĳ���Ͱ� ���� ���� ����.
	}
}

void UMainPlayerAnimInstance::AddMousePitch(float _v)
{
	if (0 != _v)
	{
		InputMousePitch += _v; // Input���� �������Ѽ� ĳ���Ͱ� ���� ���� ����.
	}
}

void UMainPlayerAnimInstance::AddMoveHorizontal(float _v)
{
	if (0 == _v)// ��,�� �������� ���� ���¶��
	{
		float wantV = FMath::FInterpTo(InputMoveHorizontal, 0.0f, GetWorld()->GetDeltaSeconds(), 5.f); // BS�� ���� 0���� ������ �ǵ�����
		InputMoveHorizontal = wantV;
	}
	else
	{
		if (InputMoveHorizontal >= 45.f)
		{
			InputMoveHorizontal = 45.f;
		}
		else if(InputMoveHorizontal <= -45.f)
		{
			InputMoveHorizontal = -45.f;
		}
		else
		{
			InputMoveHorizontal += _v * 2; // ���� �� ���� �� ������ ����
		}
	}
}

void UMainPlayerAnimInstance::AddMoveVertical(float _v)
{
	if (0 == _v) // ��, �� �������� ���� ���¶��
	{
		float wantV = FMath::FInterpTo(InputMoveVertical, 0.0f, GetWorld()->GetDeltaSeconds(), 5.f); // BS�� ���� 0���� ������ �ǵ�����
		InputMoveVertical = wantV;
	}
	else
	{
		if (InputMoveVertical >= 20.f)
		{
			InputMoveVertical = 20.f;
		}
		else if (InputMoveVertical <= -20.f)
		{
			InputMoveVertical = -20.f;
		}
		else
		{
			InputMoveVertical += _v * 2; // ���� �� ���� �� ������ ����
		}
	}
}

void UMainPlayerAnimInstance::AnimNotify_FinishAttack0()
{
	UpdateComboSettings();
}

void UMainPlayerAnimInstance::AnimNotify_FinishAttack1()
{
	UpdateComboSettings();
}

void UMainPlayerAnimInstance::AnimNotify_FinishAttack2()
{
	UpdateComboSettings();
}

void UMainPlayerAnimInstance::AnimNotify_FinishAttack3()
{
	UpdateComboSettings();
}

void UMainPlayerAnimInstance::AnimNotify_EndMoveAttack()
{
	//! MoveAttack �ִϸ��̼��� ������, MoveAttack�� ���� �ſ���.
	if (bIsAttacking)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "UMainPlayerAnimInstance::AnimNotify_EndMoveAttack()");
		AMainPlayer* Owner = Cast<AMainPlayer>(TryGetPawnOwner());
		Owner->SetIsAttacking(false);
		//TODO: �߰��� ������ �ɾ �ǰ�ȿ�� ���� �� �־�� �ؿ�.
	}
}

void UMainPlayerAnimInstance::AnimNotify_StartJump()
{
	if (bIsJumping)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "UMainPlayerAnimInstance::AnimNotify_StartJump()");
		AMainPlayer* Owner = Cast<AMainPlayer>(TryGetPawnOwner());
		Owner->Jump();
	}

}

void UMainPlayerAnimInstance::UpdateComboSettings()
{
	AMainPlayer* Owner = Cast<AMainPlayer>(TryGetPawnOwner());

	if (true == Owner->GetIsAttacking()) // ���� ���� -> recovery�� ���� ��
	{
		bAttackEnded = true; // Attack Montage�� �������ε� �� Recovery Noti�� �ɸ�.(���� recovery ���°� �ƴ� ��)
	}
	else // false == Owner->GetIsAttacking()
	{
		bAttackEnded = true;
	}
}
