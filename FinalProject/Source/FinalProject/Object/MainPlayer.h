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
	float DefaultZOffset; // �⺻��, ī�޶� �ϰ� ĳ������ Z�� ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector DefaultSocketOffset; // �⺻��, Camera Socket Offset

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimInstanceBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStateMachineComponent* SM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UMainPlayerUserWidget> WidgetClass; // �������Ʈ���� ������ ���� Ŭ����

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UMainPlayerUserWidget* WidgetInstance; // ������ ���� �ν��Ͻ�

	bool bControlSpringArmYawOnly; // ������, Player�������� �ƴ� ī�޶��� �����Ӹ� �����ϰ� ���� ��.

	UPROPERTY(BlueprintReadOnly)
	float DefaultMovementMaxWalkSpeed; // �⺻ GetCharacterMovement()->MaxWalkSpeed

	bool firstClick; // �𸮾� �����Ϳ��� �ΰ��� ù Ŭ���ÿ��� �������� �����Ǳ� ������ ���콺 ��Ŀ�� �� ù Ŭ���� ��ȿ ùŬ������ �ϱ� ���� üũ�� ����

	UPROPERTY(BlueprintReadOnly) // �ִϸ��̼� ��Ÿ�� NormalAttack��.
	UAnimMontage* AttackMontage; // �׽�Ʈ ������ �ִ� �ν��Ͻ��� �̵�.

	int AttackAnimNum; // Attack Montage Animation Number : 0~3

public: // for StateMachine
	bool bIsJumping; // Jump ���� State���� ����ϴ� Jump ���� ����
	bool bIsAttacking; // Attack ���� State���� ����ϴ� Attack ���� ����
	bool bIsSiuuuuAttacking; // ŷ��Siuuuu Attack
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
	void MoveVertical(float _v); // �÷��̾��� �̵� �Լ���
	void MoveHorizontal(float _v);

	void MousePitch(float _v); // ���콺�� �̿��� �̵� �� ���� ��ȯ �Լ���
	void MouseYaw(float _v);

	void Attack(); // attack Ű ������ �� �����ϴ� �Լ�
	void MyJump(); // jump Ű ������ �� �����ϴ� �Լ�
	virtual void Landed(const FHitResult& Hit) override; // Landed() Override

	void QAttack(); // Q Ű ������ �� �����ϴ� �Լ�
	void EAttack(); // E Ű ������ �� �����ϴ� �Լ�

	void DebugCurrentState(); // OŰ�� ������ ��, ���� State�� �˱� ���� ������ �Լ�

	UFUNCTION(BlueprintCallable) // �� �Լ��� ���� �ٲٴ� �� (�⺻���� �ٲٴ� �� �ƴ�.)
	void SetMovementMaxWalkSpeed(float _v);
	UFUNCTION(BlueprintCallable) // �� �Լ��� �⺻���� �������� ��
	float GetDefaultMovementMaxWalkSpeed() { return DefaultMovementMaxWalkSpeed; }
	
	void OnControlSpringArmYawOnly() { bControlSpringArmYawOnly = true; }
	void OffControlSpringArmYawOnly();

	//! SpringArm�� ������ Camera�� WorldPos ���ϴ� �Լ�.
	FVector GetCameraWorldLocation();

	void PlayNormalAttackMontage(); // MainPlayer�� ����Montage ���� Play �Լ�

public:
	UFUNCTION() // Montage�� ������ �� ����Ǵ� ��������Ʈ �ݹ� �Լ�.
	void OnMontageEndedCallback(UAnimMontage* Montage, bool bInterrupted);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override; // ��� ������Ʈ�� �ʱ�ȭ�� ���Ŀ� ȣ��. BeginPlay���� ���� ����.

public:
	AMainPlayer();

};
