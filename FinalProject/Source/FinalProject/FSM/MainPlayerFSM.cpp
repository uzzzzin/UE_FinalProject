// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerFSM.h"
#include "../Object/MainPlayer.h"
#include "../Component/StateMachineComponent.h"
#include "../Object/MainPlayerAnimInstance.h"

UMainPlayerFSM::UMainPlayerFSM()
{
	UE_LOG(LogTemp, Log, TEXT("UMainPlayerFSM::UMainPlayerFSM()"));
}

void UMainPlayerFSM::Initialize()
{
	if (0 == GetStateKeys().Num())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "FAIL : UMainPlayerFSM()-> State Keys Size = 0.. ");
		return;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerFSM()->Initialize() ");
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( Set Cur State ---> %s )"), *GetStateKeys()[0].ToString()));
		SetCurStateName(GetStateKeys()[0]);
	}
	//SetCurStateName(FName("Idle")); // 맨 처음은 Idle이에요.
}

void UMainPlayerFSM::FSM_Begin()
{
	Super::FSM_Begin();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerFSM::FSM_Begin()");

	// FSM이 돌아가기 시작하면,
	// 현재 Owner의 AnimInstance에게 FSM쪽에서 만든 State들이 무엇이 있는지 알려줘야 해요.
	// 그래야 다른 State 접근을 안 하니까.
	// 내가 만든 FSM의 State 기준으로 AnimInstance의 StateMachine도 돌아가게 할거예요.


	// FSM의 States 키들을 통해 AnimInstance의 State 키를 관리해요. 
	// 트랜지션 껐켜 관리는 결국 AnimInstance에서 해야 하기 때문.

	TMap<FName, bool> tmpArr;
	for (int i = 0; i < GetStateKeys().Num(); ++i)
	{
		FName tmpStateName = GetStateKeys()[i];
		tmpArr.Add(tmpStateName, false);
	}
	FName tmp = FName("Idle");
	tmpArr[tmp] = true;

	SetAnimInst(Cast<AMainPlayer>(GetOwnerStateMachine()->GetOwner())->GetMesh()->GetAnimInstance());
	Cast<UMainPlayerAnimInstance>(GetAnimInst())->SetAnimStates(tmpArr); // 현재 States들을 AnimInstance에서도 배열로 관리할 수 있게 해요.
}

void UMainPlayerFSM::FSM_Tick(float DeltaTime)
{
	Super::FSM_Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerFSM::FSM_Tick()");
}

void UMainPlayerFSM::FSM_Exit()
{
	Super::FSM_Exit();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerFSM::FSM_Exit()");
}

void UMainPlayerFSM::SetAnimInstState(FName _prev, FName _key)
{
	Cast<UMainPlayerAnimInstance>(GetAnimInst())->SetOneAnimState(_prev, false); // 이전의 것은 상태를 꺼줘요.
	Cast<UMainPlayerAnimInstance>(GetAnimInst())->SetOneAnimState(_key, true); // 바뀔 상태는 켜줘요

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( ChangeState() AnimState : %s ---> %s )"), *_prev.ToString(), *_key.ToString()));
}