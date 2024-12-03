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

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimInstanceBP;

	bool bControlSpringArmYawOnly; // 디버깅용, Player움직임이 아닌 카메라의 움직임만 제어하고 싶을 때.

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsMoving; // 지금 이동중이에요, state->move, AnimInstance에서 확인용.

	UPROPERTY(BlueprintReadOnly) 
	UAnimMontage* AttackMontage; // 애니메이션 몽타주 - Attack.

	UPROPERTY(BlueprintReadOnly)
	float AttackMontagePlayRate; // Attack Animation Montage의 기본 플레이 속도.

	UPROPERTY(BlueprintReadONly)
	bool bIsAttacking; // 지금 공격중이에요. ( == 공격 애니메이션 실행중이에요.)

	int AttackComboNum; // Attack Montage Animation Number : 0~3 (Section Name : PoleAttack%d)

public:
	void MoveVertical(float _v); // 플레이어의 이동 함수들
	void MoveHorizontal(float _v);

	void MousePitch(float _v); // 마우스를 이용한 이동 및 시점 전환 함수들
	void MouseYaw(float _v);

	void Attack(); // attack 키 눌렀을 때 동작하는 함수

	UFUNCTION(BlueprintCallable, Category = "State")
	bool GetIsMoving() { return bIsMoving; }

	UFUNCTION(BlueprintCallable, Category = "State")
	bool GetIsAttacking() { return bIsAttacking; }
	
	void OnControlSpringArmYawOnly() { bControlSpringArmYawOnly = true; }
	void OffControlSpringArmYawOnly();
	
public:
	//! Attack Montage용 Funcs.
	void PlayAttackMontage(); // MainPlayer용 공격 Montage 관련 Play 함수.

	UFUNCTION(BlueprintCallable) // 애니메이션 Notify에서 쓸거예요.
	UAnimMontage* GetAttackMontage() { return AttackMontage; }

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
