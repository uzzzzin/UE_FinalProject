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
	class UStateMachineComponent* OwnerComponent; // 내가 지금 누구 소속이게.
	TMap<FName, IState*> States; // State들을 저장해둔 곳. Key-StateName, Value-StatePointer(IState)
	TArray<FName> StateKeys; // 키만 중복없이 모아둘게요.
	FName curStateName; // 현재State의 Name. Key를 들고 있는 게 편함.

	UAnimInstance* animInst; // animInst의 것들도 호환시킬 거니까 들고 있어요.

public:
	void SetOwnerStateMachine(class UStateMachineComponent* _SM) { OwnerComponent = _SM; }
	class UStateMachineComponent* GetOwnerStateMachine() { return OwnerComponent; }
	TMap<FName, IState*>& GetStates() { return States; }
	TArray<FName>& GetStateKeys() { return StateKeys; }
	FName GetCurStateName() { return curStateName;  };
	IState* GetCurState() { return States[curStateName]; }
	void SetCurStateName(FName _name) { curStateName = _name; }
	FName GetStateNameByStateValue(IState* _stateValue); // States->Value로 Key 찾기

	void ChangeState(FName _key);
	virtual void SetAnimInstState(FName _prev, FName _key);

	void SetAnimInst(UAnimInstance* _inst) { animInst = _inst; }
	UAnimInstance* GetAnimInst() { return animInst; }
public:
	//! 템플릿으로 구현된 함수들.
	// void ChangeState(FName _key); 
	// T* FindState(Fname _key);
	// void AddState(FName _key);

	//template<typename T>
	//inline void ChangeState(FName _key) // Key를 받아야 하는 이유는, T 타입의 State가 여러개일 수 있고, 그걸 Key로 구분해야 하니까.
	//{
	//	T* tmpState = FindState<T>(_key);
	//	FName prevStateName = curStateName;

	//	//if (nullptr == FindState<T>(_key))
	//	if (nullptr == tmpState)
	//	{
	//		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : FindState() ---> %s  )"), *_key.ToString()));
	//		ChangeState<T>(curStateName); // 바꿀 State가 없는 State라면 현재 State를 다시 실행해요
	//	}
	//	else
	//	{
	//		// 1. 현재 State -> Exit
	//		// 2. 전환될 State ->Exter
	//		// 3. 현재 StateName -> 전환될 StateName
	//		States[curStateName]->Exit_Implementation();
	//		curStateName = _key;
	//		States[curStateName]->Enter_Implementation();

	//	}
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("( ChangeState() %s ---> %s  )"), *prevStateName.ToString(), *curStateName.ToString()));
	//}

	template<typename T>
	inline T* FindState(FName _key)
	{
		if (true == States.Contains(_key)) // 추가하려는 State의 Key가 있다면
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( SUCCESS : FindState() ---> %s )"), *_key.ToString()));
			return Cast<T>(States[_key]); // 찾았어요.
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : FindState() Nothing ---> %s )"), *_key.ToString()));
			return nullptr; // 없어요.
		}
	}

	//template<typename T>
	//inline void AddState(FName _key)
	//{
	//	if (nullptr != FindState<T>(_key)) // 찾아보니, 이미 States 에 들어있는데요?
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( AddState() Key ---> %s )"), *_key.ToString()));
	//		return;
	//	}

	//	// State 객체 생성해서 States에 넣어줘요.
	//	//T* wannaAddState = new T(); // 뭐가 문젠데 여기서 에러나니
	//	T* wannaAddState = NewObject<T>();
	//	States.Add(_key, wannaAddState);
	//}

public:
	virtual void Initialize();// State들 초기화, States맵 초기 관리.
public:
	virtual void FSM_Begin();
	virtual void FSM_Tick(float DeltaTime);
	virtual void FSM_Exit();

public:
	UBaseFSM();
};
