// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "../FSM/BaseFSM.h"
#include "StateMachineComponent.generated.h"

USTRUCT(BlueprintType)
struct FStateMachineData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FSM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString State;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FINALPROJECT_API UStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	TMap<FName, UBaseFSM*> StateMachines;
	TArray<FName> FSMKeys; //키만 (중복없이)모아둘게요.
	FName curFSMKey; //TODO: 키만 바뀌면 모든 걸 적용할 수 있게 구현하는 걸 목표로 해요.
	UDataTable* FSMData; // DataTable
	FString FSMDataPath;

public:
	TMap<FName, UBaseFSM*> GetStateMachines() { return StateMachines; }
	UBaseFSM* GetCurFSM() { return StateMachines[curFSMKey]; }
	UBaseFSM* GetFSMByKey(FName _key) { return StateMachines[_key]; }
	UDataTable* GetFSMData() { return FSMData; }
	TArray<FName> GetFSMKeys() { return FSMKeys; }

public:
	void SetCurFSM(FName _key) { curFSMKey = _key; }
	void SetFSMDataPath(FString _path) { FSMDataPath = _path; }

public:
	void LoadFSMData(const FString& FilePath); // CSV를 불러와 StateMachines에 데이터를 채울 거예요.

	bool ChangeFSM(FName _wannaKey);

	//template <typename T>
	//inline bool AddFSM(FName _wannaKey) //TODO: AddFSM() 은 Actor가 하는 걸로 해요.
	//{
	//	if (false == StateMachines.Contains(_wannaKey)) // Key가 없으니 FSM을 넣을 수 있어요!
	//	{
	//		// FSM 객체 생성해서 추가해줘요.
	//		T* wannaFSM = new T;
	//		StateMachines.Add(_wannaKey, wannaFSM)
	//		return true;
	//	}
	//	else
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("( FAIL : AddStateMachine() Key is Already ---> %s )"), *_wannaKey.ToString()));
	//		return false;
	//	}
	//}

public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void PostInitProperties() override;
public:	
	UStateMachineComponent();
		
};
