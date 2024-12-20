// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MiniMonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class FINALPROJECT_API AMiniMonsterAIController : public AAIController
{
	GENERATED_BODY()

private:
	FVector WannaPos; // 여기로 가고 싶어요.
	//FTimerHandle timer;

public:
	FVector GetWannaPos() { return WannaPos; }
public:
	void SetRandomPatrolLocation();

public:
	virtual void OnPossess(APawn* _owner) override;
	virtual void OnUnPossess() override;
	virtual void Tick(float dt) override;
public:
	AMiniMonsterAIController();
};
