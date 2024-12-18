// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MiniMonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FINALPROJECT_API UMiniMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	FName prevState;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, bool> AnimStates; // State Name 모음이에요.

public:
	//! StateMachine 함수들......
	UFUNCTION(BlueprintCallable, Category = "StateMachine")
	TMap<FName, bool> GetAnimStates() { return AnimStates; }
	void SetAnimStates(TMap<FName, bool> _map) { AnimStates = _map; }

	void SetOneAnimState(FName _state, bool _b) { AnimStates[_state] = _b; }

	UFUNCTION(BlueprintCallable, Category = "StateMachine")
	bool GetOneAnimStateValue(FName _state);

	FName GetPrevState() { return prevState; }
	void SetPrevState(FName _state) { prevState = _state; }

	FName GetCurState();

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UMiniMonsterAnimInstance();
};
