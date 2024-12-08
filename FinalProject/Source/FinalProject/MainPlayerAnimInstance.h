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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsSiuuuuing; // Siuuuu중인가?

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InputMoveHorizontal; // ~45, +45, BS 값 조절

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InputMoveVertical; // ~45, +45, BS 값 조절

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttackEnded; // (Attack Montage 애니메이션이 실행중이라는 전제), Attack이 아닌 Recovery 상태로 넘어갔을 떄 true
	
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle AnimTimer; // 애니메이션 관련 공용 타이머

	UPROPERTY(BlueprintReadOnly)
	float SiuuuuAttackDuration; // SiuuuuAttack시간. 사실은 SiuuuuStop 후 얼마동안 멈춰있는지에 대한 시간.

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
