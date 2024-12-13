// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../State/State.h"
#include "MainPlayerIdleState.generated.h"
/**
 * 
 */
UCLASS()
class FINALPROJECT_API UMainPlayerIdleState : public UObject, public IState
{
	GENERATED_BODY()

public:
	FTimerHandle testTimer;

public:
	bool test = false;
	bool prev = false;

	float duration = 2.f;
	float acc = 0.f;

	
public:
	virtual void Enter_Implementation() override;
	virtual void Update_Implementation(float DeltaTime) override;
	virtual void Exit_Implementation() override;

	UMainPlayerIdleState();
};
