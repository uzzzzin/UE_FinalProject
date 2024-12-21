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

void UMainPlayerAnimInstance::AddMouseYaw(float _v)
{
	if (0 != _v)
	{
		InputMouseYaw += _v; // Input값을 누적시켜서 캐릭터가 보는 각도 조절.
	}
}

void UMainPlayerAnimInstance::AddMousePitch(float _v)
{
	if (0 != _v)
	{
		InputMousePitch += _v; // Input값을 누적시켜서 캐릭터가 보는 각도 조절.
	}
}

void UMainPlayerAnimInstance::AddMoveHorizontal(float _v)
{
	if (0 == _v)// 좌,우 움직임이 없는 상태라면
	{
		float wantV = FMath::FInterpTo(InputMoveHorizontal, 0.0f, GetWorld()->GetDeltaSeconds(), 5.f); // BS의 값을 0으로 서서히 되돌리기
		InputMoveHorizontal = wantV;
	}
	else
	{
		if (InputMoveHorizontal >= 30.f)
		{
			InputMoveHorizontal = 30.f;
		}
		else if(InputMoveHorizontal <= -30.f)
		{
			InputMoveHorizontal = -30.f;
		}
		else
		{
			InputMoveHorizontal += _v * 2; // 조금 더 빠른 값 누적을 위한
		}
	}

}

void UMainPlayerAnimInstance::AddMoveVertical(float _v)
{
	if (0 == _v) // 앞, 뒤 움직임이 없는 상태라면
	{
		float wantV = FMath::FInterpTo(InputMoveVertical, 0.0f, GetWorld()->GetDeltaSeconds(), 5.f); // BS의 값을 0으로 서서히 되돌리기
		InputMoveVertical = wantV;
	}
	else
	{
		if (InputMoveVertical >= 10.f)
		{
			InputMoveVertical = 10.f;
		}
		else if (InputMoveVertical <= -10.f)
		{
			InputMoveVertical = -10.f;
		}
		else
		{
			InputMoveVertical += _v * 2; // 조금 더 빠른 값 누적을 위한
		}
	}
}

// ============================================== Notify Event ==========================================

void UMainPlayerAnimInstance::AnimNotify_EndMoveAttack()
{
	// 점프 끝내는 Notify에 들어왔으면 점프를 끝내야죠.
	if (true == Cast<AMainPlayer>(TryGetPawnOwner())->GetIsQAttacking()) // 점프중이에요.
	{
		Cast<AMainPlayer>(TryGetPawnOwner())->SetIsQAttacking(false);
	}
}

void UMainPlayerAnimInstance::AnimNotify_EndSiuuuuAttack()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "UMainPlayerAnimInstance::AnimNotify_EndSiuuuuAttack()");

	// SiuuuuAttack 끝내는 Notify에 들어왔으면 SiuuuuAttack을 끝내야죠.
	if (true == Cast<AMainPlayer>(TryGetPawnOwner())->GetIsSiuuuuAttacking()) // SiuuuuAttack중이에요.
	{
		Cast<AMainPlayer>(TryGetPawnOwner())->SetIsSiuuuuAttacking(false);
	}
}

void UMainPlayerAnimInstance::AnimNotify_FinishAttack0()
{
	AMainPlayer* owner = Cast<AMainPlayer>(TryGetPawnOwner());
	UStateMachineComponent* SM = owner->FindComponentByClass<UStateMachineComponent>();

	if (nullptr != SM)
	{
		owner->SetIsAttacking(false);
		//SM->GetCurFSM()->ChangeState(FName("PostNormalAttac"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "No StateMachineComponent found in Owner Actor!");
	}
}

void UMainPlayerAnimInstance::AnimNotify_FinishAttack1()
{
	AMainPlayer* owner = Cast<AMainPlayer>(TryGetPawnOwner());
	UStateMachineComponent* SM = owner->FindComponentByClass<UStateMachineComponent>();

	if (nullptr != SM)
	{
		owner->SetIsAttacking(false);
		//SM->GetCurFSM()->ChangeState(FName("PostNormalAttac"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "No StateMachineComponent found in Owner Actor!");
	}
}

void UMainPlayerAnimInstance::AnimNotify_FinishAttack2()
{
	AMainPlayer* owner = Cast<AMainPlayer>(TryGetPawnOwner());
	UStateMachineComponent* SM = owner->FindComponentByClass<UStateMachineComponent>();

	if (nullptr != SM)
	{
		owner->SetIsAttacking(false);
		//SM->GetCurFSM()->ChangeState(FName("PostNormalAttac"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "No StateMachineComponent found in Owner Actor!");
	}
}

void UMainPlayerAnimInstance::AnimNotify_FinishAttack3()
{
	AMainPlayer* owner = Cast<AMainPlayer>(TryGetPawnOwner());
	UStateMachineComponent* SM = owner->FindComponentByClass<UStateMachineComponent>();

	if (nullptr != SM)
	{
		owner->SetIsAttacking(false);
		//SM->GetCurFSM()->ChangeState(FName("PostNormalAttac"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "No StateMachineComponent found in Owner Actor!");
	}
}
