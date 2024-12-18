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
	if (0 == AnimStates.Num()) // 아직 초기화 안 됐는데요?
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("FAIL : GetOneAnimStateValue() AnimStates are Not Init."));
		return false;
	}

	else
	{
		if (true == AnimStates.Contains(_state))
			return AnimStates[_state]; 
		else
		{   // 찾는 State가 없는데요??
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("FAIL : GetOneAnimStateValue() ---> %s"), *_state.ToString()));
			return false;
		}
	}
}

FName UMiniMonsterAnimInstance::GetCurState()
{
	// AnimInstance는 순회를 하면서 현재 true인 상태를 찾아줘요.
	//! true인 상태 == 현재 진행중인 State

	for (const TPair<FName, bool>& s : AnimStates)
	{
		if (true == s.Value) // bool 값이 true일 때만 실행
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("GetCurState() : %s"), *(s.Key).ToString()));
			return s.Key;
		}
	}
	return FName(); // IsNone()으로 값 확인?
}

