// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MiniMonster.generated.h"

UCLASS()
class FINALPROJECT_API AMiniMonster : public ACharacter
{
	GENERATED_BODY()

private:
	int wolfStyle; // 0~2, ���ڸ��� �ɷ�ġ�� �޶��.

public:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimInstanceBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StateMachine")
	class UStateMachineComponent* SM;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	//TSubclassOf<class UMiniMonsterUserWidget> WidgetClass; // �������Ʈ���� ������ ���� Ŭ����

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	//UMiniMonsterUserWidget* WidgetInstance; // ������ ���� �ν��Ͻ�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetComponent* widgetCom;

public:
	void SetRandomMaterial(); // �������� ���͸����� ������ �ſ���.

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override; // ��� ������Ʈ�� �ʱ�ȭ�� �� �Ŀ� ȣ��. BeginPlay���� ����!

public:
	AMiniMonster();

};
