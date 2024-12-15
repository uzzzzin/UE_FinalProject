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
    //! 블루프린트과 연계시키기 위해 함수 구현 X
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State")
    void Enter(); // State 진입 시 호출되어요.

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State")
    void Update(float DeltaTime);  // State Tick, 매 프레임 호출되어요.

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State")
    void Exit(); // State 종료 시 호출되어요.

    IState();
};

