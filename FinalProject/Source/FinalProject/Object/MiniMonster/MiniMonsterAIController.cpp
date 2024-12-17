// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniMonsterAIController.h"

AMiniMonsterAIController::AMiniMonsterAIController()
{
}

void AMiniMonsterAIController::OnPossess(APawn* _owner)
{
	Super::OnPossess(_owner);
}

void AMiniMonsterAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AMiniMonsterAIController::Tick(float dt)
{
}
