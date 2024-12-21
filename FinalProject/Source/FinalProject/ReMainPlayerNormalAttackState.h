// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State/BaseState.h"
#include "ReMainPlayerNormalAttackState.generated.h"

/**
 * 
 */
UCLASS()
class FINALPROJECT_API UReMainPlayerNormalAttackState : public UBaseState
{
	GENERATED_BODY()
	
public:
	virtual void Enter() override; // State 진입 시 호출되어요.
	virtual void Update(float DeltaTime) override;  // State Tick, 매 프레임 호출되어요.
	virtual void Exit() override; // State 종료 시 호출되어요.
public:
	UReMainPlayerNormalAttackState();
};
