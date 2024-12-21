// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State/BaseState.h"
#include "ReMiniMonsterPatrolState.generated.h"

/**
 * 
 */
UCLASS()
class FINALPROJECT_API UReMiniMonsterPatrolState : public UBaseState
{
	GENERATED_BODY()
	
private:
	FTimerHandle repeatTimer; // 반복해서 돌릴 타이머
	float speed; // 전진 속도?
	float rotSpeed; // 회전 속도

public:
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;

public:
	UReMiniMonsterPatrolState();
};
