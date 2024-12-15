// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerAnimInstance.h"
#include "MainPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Component/StateMachineComponent.h"
#include "../FSM/MainPlayerFSM.h"

UMainPlayerAnimInstance::UMainPlayerAnimInstance()
{
}

void UMainPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UMainPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//FName curState = GetCurState();
}

bool UMainPlayerAnimInstance::GetOneAnimStateValue(FName _state)
{
		if (0 == AnimStates.Num())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("FAIL : GetOneAnimStateValue() AnimStates are Not Init."));
			return false;
		}

		else
		{
			if (true == AnimStates.Contains(_state))
				return AnimStates[_state];
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("FAIL : GetOneAnimStateValue() ---> %s"), *_state.ToString()));
				return false;
			}
		}
}

FName UMainPlayerAnimInstance::GetCurState()
{
	// AnimInstance�� ��ȸ�� �ϸ鼭 ���� true�� ���¸� ã�����.
	//! true�� ���� == ���� �������� State

	for (const TPair<FName, bool>& s : AnimStates)
	{
		if (true == s.Value) // bool ���� true�� ���� ����
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("GetCurState() : %s"), *(s.Key).ToString()));
			return s.Key;
		}
	}
	return FName(); // IsNone()���� �� Ȯ��?
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