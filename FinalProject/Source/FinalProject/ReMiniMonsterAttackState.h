// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State/BaseState.h"
#include "ReMiniMonsterAttackState.generated.h"

/**
 * 
 */
UCLASS()
class FINALPROJECT_API UReMiniMonsterAttackState : public UBaseState
{
	GENERATED_BODY()
public:
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;
public:
	UReMiniMonsterAttackState();
};
