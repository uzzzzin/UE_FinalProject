// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State/BaseState.h"
#include "ReMiniMonsterIdleState.generated.h"

/**
 * 
 */
UCLASS()
class FINALPROJECT_API UReMiniMonsterIdleState : public UBaseState
{
	GENERATED_BODY()
public:
	bool test = false;
	float acc = 0.f;
	float dura = 0.5f;
public:
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;
public:
	UReMiniMonsterIdleState();

};
