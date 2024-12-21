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
	int wolfStyle; // 0~2, 숫자마다 능력치가 달라요.

public:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimInstanceBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StateMachine")
	class UStateMachineComponent* SM;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	//TSubclassOf<class UMiniMonsterUserWidget> WidgetClass; // 블루프린트에서 설정할 위젯 클래스

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	//UMiniMonsterUserWidget* WidgetInstance; // 생성된 위젯 인스턴스

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetComponent* widgetCom;

public:
	void SetRandomMaterial(); // 랜덤으로 머터리얼을 배정할 거예요.

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override; // 모든 컴포넌트가 초기화된 직 후에 호출. BeginPlay보다 전에!

public:
	AMiniMonster();

};
