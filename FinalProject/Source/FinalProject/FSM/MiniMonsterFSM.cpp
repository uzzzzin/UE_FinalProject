// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniMonsterFSM.h"
#include "../Component/StateMachineComponent.h"
#include "../Object/MiniMonster/MiniMonster.h"
#include "../Object/MiniMonster/MiniMonsterAnimInstance.h"

UMiniMonsterFSM::UMiniMonsterFSM()
{
	UE_LOG(LogTemp, Log, TEXT("UMiniMonsterFSM::UMiniMonsterFSM()"));
}

void UMiniMonsterFSM::SetAnimInstState(FName _prev, FName _key)
{
	// ChangeState �� �� ����Ǵ� �Լ�����.
	Cast<UMiniMonsterAnimInstance>(GetAnimInst())->SetOneAnimState(_key, false); // ������ ���� ���¸� �����.
	Cast<UMiniMonsterAnimInstance>(GetAnimInst())->SetOneAnimState(_key, true); // �ٲ� ���´� �����

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( ChangeState() AnimState : %s ---> %s )"), *_prev.ToString(), *_key.ToString()));
}

void UMiniMonsterFSM::Initialize()
{
	if (0 == GetStateKeys().Num())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "FAIL : UMiniMonsterFSM()-> State Keys Size = 0.. ");
		return;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMiniMonsterFSM()->Initialize() ");
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( Set Cur State ---> %s )"), *GetStateKeys()[0].ToString()));
		SetCurStateName(GetStateKeys()[0]);
	}
}

void UMiniMonsterFSM::FSM_Begin()
{
	Super::FSM_Begin();

	TMap<FName, bool> tmpArr;
	for (int i = 0; i < GetStateKeys().Num(); ++i)
	{
		FName tmpStateName = GetStateKeys()[i];
		tmpArr.Add(tmpStateName, false);
	}
	FName tmp = FName("Idle"); //!? ���� �ʱ� State ����
	tmpArr[tmp] = true;

	SetAnimInst(Cast<AMiniMonster>(GetOwnerStateMachine()->GetOwner())->GetMesh()->GetAnimInstance());
	Cast<UMiniMonsterAnimInstance>(GetAnimInst())->SetAnimStates(tmpArr); // ���� States���� AnimInstance������ �迭�� ������ �� �ְ� �ؿ�.
}

void UMiniMonsterFSM::FSM_Tick(float DeltaTime)
{
	Super::FSM_Tick(DeltaTime);
}

void UMiniMonsterFSM::FSM_Exit()
{
	Super::FSM_Exit();
}

