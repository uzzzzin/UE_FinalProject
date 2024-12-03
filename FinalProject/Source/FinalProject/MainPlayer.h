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

	bool bControlSpringArmYawOnly; // ������, Player�������� �ƴ� ī�޶��� �����Ӹ� �����ϰ� ���� ��.

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsMoving; // ���� �̵����̿���, state->move, AnimInstance���� Ȯ�ο�.

	UPROPERTY(BlueprintReadOnly) // �ִϸ��̼� ��Ÿ�� �׽�Ʈ��.
	UAnimMontage* AttackMontage; // �׽�Ʈ ������ �ִ� �ν��Ͻ��� �̵�.

	int AttackAnimNum; // Attack Montage Animation Number : 0~3

public:
	void MoveVertical(float _v); // �÷��̾��� �̵� �Լ���
	void MoveHorizontal(float _v);

	void MousePitch(float _v); // ���콺�� �̿��� �̵� �� ���� ��ȯ �Լ���
	void MouseYaw(float _v);

	void Attack(); // attack Ű ������ �� �����ϴ� �Լ�

	UFUNCTION(BlueprintCallable, Category = "State")
	bool GetIsMoving() { return bIsMoving; }

	void OnControlSpringArmYawOnly() { bControlSpringArmYawOnly = true; }
	void OffControlSpringArmYawOnly();
	
public:
	void PlayAttackMontage(); // MainPlayer�� ����Montage ���� Play �Լ�

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	AMainPlayer();

};
