// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../State/State.h"
#include "MainPlayerPostMoveAttackState.generated.h"
/**
 * 
 */
UCLASS()
class FINALPROJECT_API UMainPlayerPostMoveAttackState : public UObject, public IState
{
	GENERATED_BODY()

public:
	float accTime;
	float duration;
	float Recoveryduration;

	FVector defaultCameraRelativePos;
	
public:
	virtual void Enter_Implementation() override;
	virtual void Update_Implementation(float DeltaTime) override;
	virtual void Exit_Implementation() override;

	void CameraZoomOutWithCurve(float DeltaTime);

	UMainPlayerPostMoveAttackState();
};
