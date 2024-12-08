// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"

UCLASS()
class FINALPROJECT_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USpringArmComponent* springArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCameraComponent* camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DefaultZOffset; // 기본값, 카메라 암과 캐릭터의 Z축 높이 차이

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector DefaultSocketOffset; // 기본값, Camera Socket Offset


	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimInstanceBP;

	bool bControlSpringArmYawOnly; // 디버깅용, Player움직임이 아닌 카메라의 움직임만 제어하고 싶을 때.

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsMoving; // 지금 이동중이에요, state->move, AnimInstance에서 확인용.

	UPROPERTY(BlueprintReadOnly)
	float DefaultMovementMaxWalkSpeed; // 기본 GetCharacterMovement()->MaxWalkSpeed

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsAttacking; // 지금 공격중이에요. ( == 공격 애니메이션 실행중이에요.)

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsSiuuuuAttacking; // 지금 Siuuuu 공격중이에요. ( == 공격 애니메이션 실행중이에요.)

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsJumping; // 지금 점프중이에요.

	//! Debug - State
	UPROPERTY(VisibleAnywhere, Category = "State")
	FName prevState; // 1틱 전 State
	UPROPERTY(VisibleAnywhere, Category = "State")
	FName curState; // 현재 State

	bool prevIsAttacking; // 디버깅용 현재 Attacking 값 확인

public:
	void MoveVertical(float _v); // 플레이어의 이동 함수들
	void MoveHorizontal(float _v);

	void MousePitch(float _v); // 마우스를 이용한 이동 및 시점 전환 함수들
	void MouseYaw(float _v);

	void Attack(); // attack 키 눌렀을 때 동작하는 함수
	void MyJump(); // jump 키 눌렀을 때 동작하는 함수
	virtual void Landed(const FHitResult& Hit) override; // Landed() Override

	void DebugCurrentState(); // O키를 눌렀을 때, 현재 State를 알기 위한 디버깅용 함수

	//! StateMachine - State
	UFUNCTION(BlueprintCallable, Category = "State")
	bool GetIsMoving() { return bIsMoving; }

	UFUNCTION(BlueprintCallable, Category = "State")
	void SetIsAttacking(bool _v) { bIsAttacking = _v; }
	UFUNCTION(BlueprintCallable, Category = "State")
	bool GetIsAttacking() { return bIsAttacking; }

	UFUNCTION(BlueprintCallable, Category = "State")
	bool GetIsSiuuuuAttacking() { return bIsSiuuuuAttacking; }
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetIsSiuuuuAttacking(bool _v) { bIsSiuuuuAttacking = _v; }

	UFUNCTION(BlueprintCallable, Category = "State")
	void SetIsJumping(bool _v) { bIsJumping = _v; }
	UFUNCTION(BlueprintCallable, Category = "State")
	bool GetIsJumping() { return bIsJumping; }


	UFUNCTION(BlueprintCallable) // 이 함수는 값을 바꾸는 것 (기본값을 바꾸는 게 아님.)
	void SetMovementMaxWalkSpeed(float _v);
	UFUNCTION(BlueprintCallable) // 이 함수는 기본값을 가져오는 것
	float GetDefaultMovementMaxWalkSpeed() { return DefaultMovementMaxWalkSpeed; }
	
	void OnControlSpringArmYawOnly() { bControlSpringArmYawOnly = true; }
	void OffControlSpringArmYawOnly();

	//! SpringArm에 부착된 Camera의 WorldPos 구하는 함수.
	FVector GetCameraWorldLocation();
	
public:
	UFUNCTION() // Montage가 끝났을 떄 실행되는 델리게이트 콜백 함수.
	void OnMontageEndedCallback(UAnimMontage* Montage, bool bInterrupted);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override; // 모든 컴포넌트가 초기화된 직후에 호출. BeginPlay보다 전에 실행.

public:
	AMainPlayer();

};
