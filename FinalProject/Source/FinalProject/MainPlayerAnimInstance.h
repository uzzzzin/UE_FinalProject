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

	// 카테고리가 State라면, AnimGraph에서 쓰이는 state 관련 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsMoving; // 이동중인가?

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsAttacking; // 이동중인가?

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsJumping; // 점핑중인가?

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InputMoveHorizontal; // ~45, +45, BS 값 조절

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InputMoveVertical; // ~45, +45, BS 값 조절

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttackEnded; // (Attack Montage 애니메이션이 실행중이라는 전제), Attack이 아닌 Recovery 상태로 넘어갔을 떄 true
	
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
	void UpdateComboSettings(); // bAttackEnded 세팅.

	UFUNCTION(BlueprintCallable)
	bool GetbAttackEnded() { return bAttackEnded; }
	UFUNCTION(BlueprintCallable)
	void SetbAttackEnded(bool _b) { bAttackEnded = _b; }

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UMainPlayerAnimInstance();
};
