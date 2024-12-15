// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../State/State.h"
#include "BaseFSM.generated.h"

/**
 * 
 */
UCLASS()
class FINALPROJECT_API UBaseFSM : public UObject
{
	GENERATED_BODY()
	
private:
	class UStateMachineComponent* OwnerComponent; // ���� ���� ���� �Ҽ��̰�.
	TMap<FName, IState*> States; // State���� �����ص� ��. Key-StateName, Value-StatePointer(IState)
	TArray<FName> StateKeys; // Ű�� �ߺ����� ��ƵѰԿ�.
	FName curStateName; // ����State�� Name. Key�� ��� �ִ� �� ����.

	UAnimInstance* animInst; // animInst�� �͵鵵 ȣȯ��ų �Ŵϱ� ��� �־��.

public:
	void SetOwnerStateMachine(class UStateMachineComponent* _SM) { OwnerComponent = _SM; }
	class UStateMachineComponent* GetOwnerStateMachine() { return OwnerComponent; }
	TMap<FName, IState*>& GetStates() { return States; }
	TArray<FName>& GetStateKeys() { return StateKeys; }
	FName GetCurStateName() { return curStateName;  };
	IState* GetCurState() { return States[curStateName]; }
	void SetCurStateName(FName _name) { curStateName = _name; }
	FName GetStateNameByStateValue(IState* _stateValue); // States->Value�� Key ã��

	void ChangeState(FName _key);
	virtual void SetAnimInstState(FName _prev, FName _key);

	void SetAnimInst(UAnimInstance* _inst) { animInst = _inst; }
	UAnimInstance* GetAnimInst() { return animInst; }
public:
	//! ���ø����� ������ �Լ���.
	// void ChangeState(FName _key); 
	// T* FindState(Fname _key);
	// void AddState(FName _key);

	//template<typename T>
	//inline void ChangeState(FName _key) // Key�� �޾ƾ� �ϴ� ������, T Ÿ���� State�� �������� �� �ְ�, �װ� Key�� �����ؾ� �ϴϱ�.
	//{
	//	T* tmpState = FindState<T>(_key);
	//	FName prevStateName = curStateName;

	//	//if (nullptr == FindState<T>(_key))
	//	if (nullptr == tmpState)
	//	{
	//		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : FindState() ---> %s  )"), *_key.ToString()));
	//		ChangeState<T>(curStateName); // �ٲ� State�� ���� State��� ���� State�� �ٽ� �����ؿ�
	//	}
	//	else
	//	{
	//		// 1. ���� State -> Exit
	//		// 2. ��ȯ�� State ->Exter
	//		// 3. ���� StateName -> ��ȯ�� StateName
	//		States[curStateName]->Exit_Implementation();
	//		curStateName = _key;
	//		States[curStateName]->Enter_Implementation();

	//	}
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("( ChangeState() %s ---> %s  )"), *prevStateName.ToString(), *curStateName.ToString()));
	//}

	template<typename T>
	inline T* FindState(FName _key)
	{
		if (true == States.Contains(_key)) // �߰��Ϸ��� State�� Key�� �ִٸ�
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( SUCCESS : FindState() ---> %s )"), *_key.ToString()));
			return Cast<T>(States[_key]); // ã�Ҿ��.
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : FindState() Nothing ---> %s )"), *_key.ToString()));
			return nullptr; // �����.
		}
	}

	//template<typename T>
	//inline void AddState(FName _key)
	//{
	//	if (nullptr != FindState<T>(_key)) // ã�ƺ���, �̹� States �� ����ִµ���?
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( AddState() Key ---> %s )"), *_key.ToString()));
	//		return;
	//	}

	//	// State ��ü �����ؼ� States�� �־����.
	//	//T* wannaAddState = new T(); // ���� ������ ���⼭ ��������
	//	T* wannaAddState = NewObject<T>();
	//	States.Add(_key, wannaAddState);
	//}

public:
	virtual void Initialize();// State�� �ʱ�ȭ, States�� �ʱ� ����.
public:
	virtual void FSM_Begin();
	virtual void FSM_Tick(float DeltaTime);
	virtual void FSM_Exit();

public:
	UBaseFSM();
};
