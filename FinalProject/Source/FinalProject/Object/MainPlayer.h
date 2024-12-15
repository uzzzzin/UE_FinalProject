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

	bool bControlSpringArmYawOnly; // ������, Player�������� �ƴ� ī�޶��� �����Ӹ� �����ϰ� ���� ��.

	UPROPERTY(BlueprintReadOnly)
	float DefaultMovementMaxWalkSpeed; // �⺻ GetCharacterMovement()->MaxWalkSpeed

	bool firstClick; // �𸮾� �����Ϳ��� �ΰ��� ù Ŭ���ÿ��� �������� �����Ǳ� ������ ���콺 ��Ŀ�� �� ù Ŭ���� ��ȿ ùŬ������ �ϱ� ���� üũ�� ����

public: // for StateMachine
	bool bIsJumping; // Jump ���� State���� ����ϴ� Jump ���� ����
	bool bIsAttacking; // Attack ���� State���� ����ϴ� Attack ���� ����

public:
	bool GetIsJumping() { return bIsJumping; }
	bool GetIsAttacking() { return bIsAttacking; }
	void SetIsAttacking(bool _b) { bIsAttacking = _b; }
public:
	void MoveVertical(float _v); // �÷��̾��� �̵� �Լ���
	void MoveHorizontal(float _v);

	void MousePitch(float _v); // ���콺�� �̿��� �̵� �� ���� ��ȯ �Լ���
	void MouseYaw(float _v);

	void Attack(); // attack Ű ������ �� �����ϴ� �Լ�
	void MyJump(); // jump Ű ������ �� �����ϴ� �Լ�
	virtual void Landed(const FHitResult& Hit) override; // Landed() Override

	void DebugCurrentState(); // OŰ�� ������ ��, ���� State�� �˱� ���� ������ �Լ�

	UFUNCTION(BlueprintCallable) // �� �Լ��� ���� �ٲٴ� �� (�⺻���� �ٲٴ� �� �ƴ�.)
	void SetMovementMaxWalkSpeed(float _v);
	UFUNCTION(BlueprintCallable) // �� �Լ��� �⺻���� �������� ��
	float GetDefaultMovementMaxWalkSpeed() { return DefaultMovementMaxWalkSpeed; }
	
	void OnControlSpringArmYawOnly() { bControlSpringArmYawOnly = true; }
	void OffControlSpringArmYawOnly();

	//! SpringArm�� ������ Camera�� WorldPos ���ϴ� �Լ�.
	FVector GetCameraWorldLocation();

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
