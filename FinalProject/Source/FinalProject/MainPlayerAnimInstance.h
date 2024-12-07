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

	// ī�װ��� State���, AnimGraph���� ���̴� state ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsMoving; // �̵����ΰ�?

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsAttacking; // �̵����ΰ�?

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsJumping; // �������ΰ�?

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InputMoveHorizontal; // ~45, +45, BS �� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InputMoveVertical; // ~45, +45, BS �� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttackEnded; // (Attack Montage �ִϸ��̼��� �������̶�� ����), Attack�� �ƴ� Recovery ���·� �Ѿ�� �� true
	
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

	//! Attack Montage Notify
	UFUNCTION(BlueprintCallable)
	void AnimNotify_FinishAttack0();
	UFUNCTION(BlueprintCallable)
	void AnimNotify_FinishAttack1();
	UFUNCTION(BlueprintCallable)
	void AnimNotify_FinishAttack2();
	UFUNCTION(BlueprintCallable)
	void AnimNotify_FinishAttack3();

	//! Jump Sequence Notify
	UFUNCTION(BlueprintCallable)
	void AnimNotify_StartJump();

	UFUNCTION()
	void UpdateComboSettings(); // bAttackEnded ����.

	UFUNCTION(BlueprintCallable)
	bool GetbAttackEnded() { return bAttackEnded; }
	UFUNCTION(BlueprintCallable)
	void SetbAttackEnded(bool _b) { bAttackEnded = _b; }

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UMainPlayerAnimInstance();
};
