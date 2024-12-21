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
	virtual void Enter() override; // State ���� �� ȣ��Ǿ��.
	virtual void Update(float DeltaTime) override;  // State Tick, �� ������ ȣ��Ǿ��.
	virtual void Exit() override; // State ���� �� ȣ��Ǿ��.
public:
	UReMainPlayerNormalAttackState();
};
