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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStateMachineComponent* SM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UMainPlayerUserWidget> WidgetClass; // 블루프린트에서 설정할 위젯 클래스

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UMainPlayerUserWidget* WidgetInstance; // 생성된 위젯 인스턴스

	bool bControlSpringArmYawOnly; // 디버깅용, Player움직임이 아닌 카메라의 움직임만 제어하고 싶을 때.

	UPROPERTY(BlueprintReadOnly)
	float DefaultMovementMaxWalkSpeed; // 기본 GetCharacterMovement()->MaxWalkSpeed

	bool firstClick; // 언리얼 에디터에서 인게임 첫 클릭시에도 공격으로 인정되기 때문에 마우스 포커싱 후 첫 클릭을 유효 첫클릭으로 하기 위한 체크용 변수

	UPROPERTY(BlueprintReadOnly) // 애니메이션 몽타주 NormalAttack용.
	UAnimMontage* AttackMontage; // 테스트 끝나고 애님 인스턴스로 이동.

	int AttackAnimNum; // Attack Montage Animation Number : 0~3

public: // for StateMachine
	bool bIsJumping; // Jump 관련 State에서 사용하는 Jump 상태 변수
	bool bIsAttacking; // Attack 관련 State에서 사용하는 Attack 상태 변수
	bool bIsSiuuuuAttacking; // 킹갓Siuuuu Attack
	bool bIsQAttacking;


public:
	bool GetIsJumping() { return bIsJumping; }

	bool GetIsAttacking() { return bIsAttacking; }
	void SetIsAttacking(bool _b) { bIsAttacking = _b; }

	bool GetIsSiuuuuAttacking() { return bIsSiuuuuAttacking; }
	void SetIsSiuuuuAttacking(bool _b) { bIsSiuuuuAttacking = _b; }

	bool GetIsQAttacking() { return bIsQAttacking; }
	void SetIsQAttacking(bool _b) { bIsQAttacking = _b; }

public:
	void MoveVertical(float _v); // 플레이어의 이동 함수들
	void MoveHorizontal(float _v);

	void MousePitch(float _v); // 마우스를 이용한 이동 및 시점 전환 함수들
	void MouseYaw(float _v);

	void Attack(); // attack 키 눌렀을 때 동작하는 함수
	void MyJump(); // jump 키 눌렀을 때 동작하는 함수
	virtual void Landed(const FHitResult& Hit) override; // Landed() Override

	void QAttack(); // Q 키 눌렀을 떄 동작하는 함수
	void EAttack(); // E 키 눌렀을 떄 동작하는 함수

	void DebugCurrentState(); // O키를 눌렀을 때, 현재 State를 알기 위한 디버깅용 함수

	UFUNCTION(BlueprintCallable) // 이 함수는 값을 바꾸는 것 (기본값을 바꾸는 게 아님.)
	void SetMovementMaxWalkSpeed(float _v);
	UFUNCTION(BlueprintCallable) // 이 함수는 기본값을 가져오는 것
	float GetDefaultMovementMaxWalkSpeed() { return DefaultMovementMaxWalkSpeed; }
	
	void OnControlSpringArmYawOnly() { bControlSpringArmYawOnly = true; }
	void OffControlSpringArmYawOnly();

	//! SpringArm에 부착된 Camera의 WorldPos 구하는 함수.
	FVector GetCameraWorldLocation();

	void PlayNormalAttackMontage(); // MainPlayer용 공격Montage 관련 Play 함수

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
