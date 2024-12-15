// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "State.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UState : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IState
{
	GENERATED_BODY()

private:
    class UBaseFSM* OwnerFSM;

public:
    void SetOwnerFSM(UBaseFSM* _fsm) { OwnerFSM = _fsm; }
    class UBaseFSM* GetOwnerFSM() { return OwnerFSM; }

public:
    //! �������Ʈ�� �����Ű�� ���� �Լ� ���� X
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State")
    void Enter(); // State ���� �� ȣ��Ǿ��.

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State")
    void Update(float DeltaTime);  // State Tick, �� ������ ȣ��Ǿ��.

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State")
    void Exit(); // State ���� �� ȣ��Ǿ��.

    IState();
};

