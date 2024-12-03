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

	UPROPERTY(BlueprintReadOnly) // 애니메이션 몽타주 테스트용.
	UAnimMontage* AttackMontage; // 테스트 끝나고 애님 인스턴스로 이동.

	int AttackAnimNum; // Attack Montage Animation Number : 0~3

public:
	void MoveVertical(float _v); // 플레이어의 이동 함수들
	void MoveHorizontal(float _v);

	void MousePitch(float _v); // 마우스를 이용한 이동 및 시점 전환 함수들
	void MouseYaw(float _v);

	void Attack(); // attack 키 눌렀을 때 동작하는 함수

	UFUNCTION(BlueprintCallable, Category = "State")
	bool GetIsMoving() { return bIsMoving; }

	void OnControlSpringArmYawOnly() { bControlSpringArmYawOnly = true; }
	void OffControlSpringArmYawOnly();
	
public:
	void PlayAttackMontage(); // MainPlayer용 공격Montage 관련 Play 함수

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	AMainPlayer();

};
