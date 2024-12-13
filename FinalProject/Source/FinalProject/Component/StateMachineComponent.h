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
	TArray<FName> FSMKeys; //Ű�� (�ߺ�����)��ƵѰԿ�.
	FName curFSMKey; //TODO: Ű�� �ٲ�� ��� �� ������ �� �ְ� �����ϴ� �� ��ǥ�� �ؿ�.
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
	void LoadFSMData(const FString& FilePath); // CSV�� �ҷ��� StateMachines�� �����͸� ä�� �ſ���.

	bool ChangeFSM(FName _wannaKey);

	//template <typename T>
	//inline bool AddFSM(FName _wannaKey) //TODO: AddFSM() �� Actor�� �ϴ� �ɷ� �ؿ�.
	//{
	//	if (false == StateMachines.Contains(_wannaKey)) // Key�� ������ FSM�� ���� �� �־��!
	//	{
	//		// FSM ��ü �����ؼ� �߰������.
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
