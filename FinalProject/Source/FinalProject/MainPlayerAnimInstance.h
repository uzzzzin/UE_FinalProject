// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainPlayerAnimInstance.generated.h"

UCLASS()
class FINALPROJECT_API UMainPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InputMouseYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InputMousePitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMoving; // 이동중인가?

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InputMoveHorizontal; // ~45, +45, BS 값 조절

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InputMoveVertical; // ~45, +45, BS 값 조절
	
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
	void SetMoveHorizontal(float _v);
	UFUNCTION(BlueprintCallable, Category = "InputMove")
	void SetMoveVertical(float _v);

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UMainPlayerAnimInstance();
};
