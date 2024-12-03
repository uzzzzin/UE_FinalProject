// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerAnimInstance.h"
#include "MainPlayer.h"

UMainPlayerAnimInstance::UMainPlayerAnimInstance()
	: bIsMoving(false)
{
}

void UMainPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMainPlayer* Owner = Cast<AMainPlayer>(TryGetPawnOwner());
	if (Owner)
	{
		bIsMoving = Owner->GetIsMoving();

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