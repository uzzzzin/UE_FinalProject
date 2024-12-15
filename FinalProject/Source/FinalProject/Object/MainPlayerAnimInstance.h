// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainPlayerAnimInstance.generated.h"

UCLASS()
class FINALPROJECT_API UMainPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:
	FName prevState;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, bool> AnimStates; // State Name ����.
	float acc = 0.f;
	float dura = 3.f;
	bool test = false;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InputMouseYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InputMousePitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InputMoveHorizontal; // ~45, +45, BS �� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InputMoveVertical; // ~45, +45, BS �� ����

public:
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
	UFUNCTION(BlueprintCallable, Category = "InputMouse")
	void AddMouseYaw(float _v);
	UFUNCTION(BlueprintCallable, Category = "InputMouse")
	float GetMouseYaw() { return InputMouseYaw;  }
	UFUNCTION(BlueprintCallable, Category = "InputMouse")
	void AddMousePitch(float _v);
	UFUNCTION(BlueprintCallable, Category = "InputMouse")
	float GetMousePitch() { return InputMousePitch; }

	UFUNCTION(BlueprintCallable, Category = "InputMove")
	void AddMoveHorizontal(float _v);
	UFUNCTION(BlueprintCallable, Category = "InputMove")
	void AddMoveVertical(float _v);

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UMainPlayerAnimInstance();
};