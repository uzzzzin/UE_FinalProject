// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "State.h"
#include "MiniMonsterTraceState.generated.h"
/**
 * 
 */
UCLASS()
class FINALPROJECT_API UMiniMonsterTraceState : public UObject, public IState
{
	GENERATED_BODY()
	
public:
	virtual void Enter_Implementation() override;
	virtual void Update_Implementation(float DeltaTime) override;
	virtual void Exit_Implementation() override;

public:
	UMiniMonsterTraceState();
};