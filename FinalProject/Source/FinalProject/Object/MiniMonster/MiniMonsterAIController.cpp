// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniMonsterAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AMiniMonsterAIController::AMiniMonsterAIController()
{
}

void AMiniMonsterAIController::OnPossess(APawn* _owner)
{
	Super::OnPossess(_owner);

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Orange, "FAIL : SetRandomPatrolLocation() ---> Cant Found NavSystem.");
	}

	WannaPos = GetPawn()->GetActorLocation();
}

void AMiniMonsterAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AMiniMonsterAIController::Tick(float dt)
{
	Super::Tick(dt);
}

void AMiniMonsterAIController::SetRandomPatrolLocation()
{
	//! ���� ��ġ�� �����ϰ� ���� �־����.
	//! State<Patrol> �� �� �ݺ������� ����Ǿ��.
	FNavLocation pos = FNavLocation();

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr != NavSystem)
	{
		FVector prevPos = GetPawn()->GetActorLocation();
		NavSystem->GetRandomPointInNavigableRadius(GetPawn()->GetActorLocation(), 500.f, pos);
		WannaPos = pos.Location;
		WannaPos.Z = prevPos.Z; // Z���� ��Ź�����.
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT(" %f, %f, %f"), WannaPos.X, WannaPos.Y, WannaPos.Z));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "FAIL : SetRandomPatrolLocation() ---> Cant Found NavSystem.");
		return; // NavSystem == nullPtr �̴ϱ� �ƹ� �͵� �� �� �����.
	}
}