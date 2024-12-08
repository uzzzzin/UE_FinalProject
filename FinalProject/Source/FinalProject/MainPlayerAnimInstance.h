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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsSiuuuuing; // Siuuuu���ΰ�?

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InputMoveHorizontal; // ~45, +45, BS �� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InputMoveVertical; // ~45, +45, BS �� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttackEnded; // (Attack Montage �ִϸ��̼��� �������̶�� ����), Attack�� �ƴ� Recovery ���·� �Ѿ�� �� true
	
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle AnimTimer; // �ִϸ��̼� ���� ���� Ÿ�̸�

	UPROPERTY(BlueprintReadOnly)
	float SiuuuuAttackDuration; // SiuuuuAttack�ð�. ����� SiuuuuStop �� �󸶵��� �����ִ����� ���� �ð�.

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

	UFUNCTION(BlueprintCallable)
	void AnimNotify_EndIdleAttack();

	//! Animation Sequence Notify
	//! Move Attack
	UFUNCTION(BlueprintCallable)
	void AnimNotify_EndMoveAttack();
	UFUNCTION(BlueprintCallable)
	void DelayEndMoveAttack(); // EndMoveAttack - Timer CallBack Func

	UFUNCTION(BlueprintCallable)
	void AnimNotify_StartJump();

	//! Siuuuu Attack
	UFUNCTION(BlueprintCallable)
	void AnimNotify_StartSiuuuuStop();
	UFUNCTION(BlueprintCallable)
	void AfterTimerSiuuuuStop(); // SiuuuuAttack Timer CallBack Func

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UMainPlayerAnimInstance();
};
