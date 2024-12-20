// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "State.h"
#include "MiniMonsterPatrolState.generated.h"
/**
 * 
 */
UCLASS()
class FINALPROJECT_API UMiniMonsterPatrolState : public UObject, public IState
{
	GENERATED_BODY()
	
private:
	FTimerHandle repeatTimer; // 반복해서 돌릴 타이머
	float speed; // 전진 속도?
	float rotSpeed; // 회전 속도

public:
	virtual void Enter_Implementation() override;
	virtual void Update_Implementation(float DeltaTime) override;
	virtual void Exit_Implementation() override;

public:
	UMiniMonsterPatrolState();
};
