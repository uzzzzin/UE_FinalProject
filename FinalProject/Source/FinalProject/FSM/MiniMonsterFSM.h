// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseFSM.h"
#include "MiniMonsterFSM.generated.h"

/**
 * 
 */
UCLASS()
class FINALPROJECT_API UMiniMonsterFSM : public UBaseFSM
{
	GENERATED_BODY()
	
public:
	virtual void SetAnimInstState(FName _prev, FName _key);
public:
	virtual void Initialize() override;
	virtual void FSM_Begin() override;
	virtual void FSM_Tick(float DeltaTime) override;
	virtual void FSM_Exit() override;

public:
	UMiniMonsterFSM();
};
