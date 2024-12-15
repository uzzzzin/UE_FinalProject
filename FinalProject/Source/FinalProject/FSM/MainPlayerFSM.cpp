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
	//SetCurStateName(FName("Idle")); // �� ó���� Idle�̿���.
}

void UMainPlayerFSM::FSM_Begin()
{
	Super::FSM_Begin();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMainPlayerFSM::FSM_Begin()");

	// FSM�� ���ư��� �����ϸ�,
	// ���� Owner�� AnimInstance���� FSM�ʿ��� ���� State���� ������ �ִ��� �˷���� �ؿ�.
	// �׷��� �ٸ� State ������ �� �ϴϱ�.
	// ���� ���� FSM�� State �������� AnimInstance�� StateMachine�� ���ư��� �Ұſ���.


	// FSM�� States Ű���� ���� AnimInstance�� State Ű�� �����ؿ�. 
	// Ʈ������ ���� ������ �ᱹ AnimInstance���� �ؾ� �ϱ� ����.

	TMap<FName, bool> tmpArr;
	for (int i = 0; i < GetStateKeys().Num(); ++i)
	{
		FName tmpStateName = GetStateKeys()[i];
		tmpArr.Add(tmpStateName, false);
	}
	FName tmp = FName("Idle");
	tmpArr[tmp] = true;

	SetAnimInst(Cast<AMainPlayer>(GetOwnerStateMachine()->GetOwner())->GetMesh()->GetAnimInstance());
	Cast<UMainPlayerAnimInstance>(GetAnimInst())->SetAnimStates(tmpArr); // ���� States���� AnimInstance������ �迭�� ������ �� �ְ� �ؿ�.
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
	Cast<UMainPlayerAnimInstance>(GetAnimInst())->SetOneAnimState(_prev, false); // ������ ���� ���¸� �����.
	Cast<UMainPlayerAnimInstance>(GetAnimInst())->SetOneAnimState(_key, true); // �ٲ� ���´� �����

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( ChangeState() AnimState : %s ---> %s )"), *_prev.ToString(), *_key.ToString()));
}