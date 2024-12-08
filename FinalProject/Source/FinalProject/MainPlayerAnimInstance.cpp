// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerAnimInstance.h"
#include "MainPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

UMainPlayerAnimInstance::UMainPlayerAnimInstance()
	: bIsMoving(false)
	, bIsAttacking(false)
	, bIsSiuuuuAttacking(false)
	, bIsSiuuuuTiming(false)
	, SiuuuuAttackDuration(1.f)
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
		bIsSiuuuuAttacking = Owner->GetIsSiuuuuAttacking();

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

	if (true == bIsSiuuuuTiming)
	{
		//TODO: Siuuuu Ÿ�̸� ���ư��� ���ȿ��� �����ؿ�. ���� ����ü �ʹٴٴ� ���� ����.
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
void UMainPlayerAnimInstance::AnimNotify_EndIdleAttack()
{
	AMainPlayer* Owner = Cast<AMainPlayer>(TryGetPawnOwner());
	Owner->SetIsAttacking(false);
	Owner->SetMovementMaxWalkSpeed(Owner->GetDefaultMovementMaxWalkSpeed());
}

void UMainPlayerAnimInstance::AnimNotify_EndMoveAttack()
{
	//! MoveAttack �ִϸ��̼��� ������, MoveAttack�� ���� �ſ���.
	if (bIsAttacking)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, "UMainPlayerAnimInstance::AnimNotify_EndMoveAttack()");
		//TODO: �߰��� ������ �ɾ ���� �� ȿ�� ���� �� �־�� �ؿ�.
		AMainPlayer* Owner = Cast<AMainPlayer>(TryGetPawnOwner());
		Owner->GetCharacterMovement()->MaxWalkSpeed = 0.0f; // Ÿ�̸� �ð����� ĳ���Ͱ� �������� ���� �ſ���.
		GetWorld()->GetTimerManager().SetTimer(AnimTimer, this, &UMainPlayerAnimInstance::DelayEndMoveAttack, 1.f,false);
	}
}

void UMainPlayerAnimInstance::DelayEndMoveAttack()
{
	//! Ÿ�̸Ӱ� ������ MoveAttack State�� ����.
	//! �ٽ� ĳ���Ͱ� ������ �� �ֵ���, 0���� �ٲ�� Max Walk Speed�� �⺻������ �ǵ����ش�.
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, "UMainPlayerAnimInstance::DelayEndMoveAttack()");
	AMainPlayer* Owner = Cast<AMainPlayer>(TryGetPawnOwner());
	Owner->SetIsAttacking(false);
	Owner->SetMovementMaxWalkSpeed(Owner->GetDefaultMovementMaxWalkSpeed()); 
	GetWorld()->GetTimerManager().ClearTimer(AnimTimer);
}

void UMainPlayerAnimInstance::AnimNotify_StartJump()
{
	if (bIsJumping)
	{
		AMainPlayer* Owner = Cast<AMainPlayer>(TryGetPawnOwner());
		Owner->Jump();
	}
}

void UMainPlayerAnimInstance::AnimNotify_StartSiuuuuAttack()
{
}

void UMainPlayerAnimInstance::AnimNotify_EndSiuuuuAttack()
{
	bIsSiuuuuTiming = true; // ���� SiuuuuStop���� �Ѿ��.
}

void UMainPlayerAnimInstance::AnimNotify_StartSiuuuuStop()
{
	//TODO: Siuuuu Stop Timer �۵����Ѿ� �ؿ�
	AMainPlayer* Owner = Cast<AMainPlayer>(TryGetPawnOwner());
	Owner->GetCharacterMovement()->MaxWalkSpeed = 0.0f; // Ÿ�̸� �ð����� ĳ���Ͱ� �������� ���� �ſ���.
	GetWorld()->GetTimerManager().SetTimer(AnimTimer, this, &UMainPlayerAnimInstance::AfterTimerSiuuuuStop, SiuuuuAttackDuration, false);
}

void UMainPlayerAnimInstance::AfterTimerSiuuuuStop()
{
	AMainPlayer* Owner = Cast<AMainPlayer>(TryGetPawnOwner());
	Owner->SetMovementMaxWalkSpeed(Owner->GetDefaultMovementMaxWalkSpeed());

	bIsSiuuuuTiming = false; // Ÿ�̸� �������ϱ� ���� Siuuuu Ÿ���� �������.
	Owner->SetIsSiuuuuAttacking(false); // SiuuuuAttack�� ������.
	Owner->SetIsAttacking(false); // Siuuuu Attack �� ��� �������� ������ False �� �Ǿ��.
	GetWorld()->GetTimerManager().ClearTimer(AnimTimer);

}