// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseState.generated.h"

/**
 * 
 */
UCLASS()
class FINALPROJECT_API UBaseState : public UObject
{
	GENERATED_BODY()
	
private:
    class UBaseFSM* OwnerFSM;

public:
    void SetOwnerFSM(UBaseFSM* _fsm) { OwnerFSM = _fsm; }
    class UBaseFSM* GetOwnerFSM() { return OwnerFSM; }

public:
    UFUNCTION(BlueprintCallable, Category = "State")
    virtual void Enter(); // State ���� �� ȣ��Ǿ��.

    UFUNCTION(BlueprintCallable, Category = "State")
    virtual void Update(float DeltaTime);  // State Tick, �� ������ ȣ��Ǿ��.

    UFUNCTION(BlueprintCallable, Category = "State")
    virtual void Exit(); // State ���� �� ȣ��Ǿ��.

	UBaseState();
};
