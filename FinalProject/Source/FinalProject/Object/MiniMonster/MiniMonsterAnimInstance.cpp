// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniMonsterAnimInstance.h"

UMiniMonsterAnimInstance::UMiniMonsterAnimInstance()
{
}

void UMiniMonsterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UMiniMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

bool UMiniMonsterAnimInstance::GetOneAnimStateValue(FName _state)
{
	if (0 == AnimStates.Num()) // ���� �ʱ�ȭ �� �ƴµ���?
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("FAIL : GetOneAnimStateValue() AnimStates are Not Init."));
		return false;
	}

	else
	{
		if (true == AnimStates.Contains(_state))
			return AnimStates[_state]; 
		else
		{   // ã�� State�� ���µ���??
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("FAIL : GetOneAnimStateValue() ---> %s"), *_state.ToString()));
			return false;
		}
	}
}

FName UMiniMonsterAnimInstance::GetCurState()
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

