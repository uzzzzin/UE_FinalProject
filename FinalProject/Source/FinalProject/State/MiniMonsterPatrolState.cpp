// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniMonsterPatrolState.h"
#include "../Object/MiniMonster/MiniMonster.h"
#include "../Component/StateMachineComponent.h"
#include "../FSM/MiniMonsterFSM.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "../Object/MiniMonster/MiniMonsterAIController.h"

UMiniMonsterPatrolState::UMiniMonsterPatrolState()
	: speed(200.f)
	, rotSpeed(3.f)
{
}

void UMiniMonsterPatrolState::Enter_Implementation()
{
	AMiniMonster* owner = Cast<AMiniMonster>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());
	AMiniMonsterAIController* AI = Cast<AMiniMonsterAIController>(owner->GetController());

	//GetWorld()->GetTimerManager().SetTimer(repeatTimer, AI, &AMiniMonsterAIController::SetRandomPatrolLocation,3.f,true);

	speed += FMath::RandRange(-20.f, 30.f); // 랜덤한 스피드 값을 더해줘요.
}

void UMiniMonsterPatrolState::Update_Implementation(float DeltaTime)
{
	// Trace Trigger까지 걸어가요
	// TraceTrigger에서 ChangeState 예정.
	AMiniMonster* owner = Cast<AMiniMonster>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());
	FVector newPos = owner->GetActorLocation() + -owner->GetActorForwardVector() * speed * DeltaTime;
	owner->SetActorLocation(newPos);

	if (1600.f != owner->GetActorLocation().Z) // 땅에서 붕뜨는 현상을 막기 위한 예외처리.
		owner->SetActorLocation(FVector(owner->GetActorLocation().X, owner->GetActorLocation().Y, 1600.f));
	//AMiniMonsterAIController* AI = Cast<AMiniMonsterAIController>(owner->GetController());
	//
	//if (nullptr == AI) // AI가 아직 없어요?
	//	return;
	//else
	//{
	//	APawn* ControlledPawn = AI->GetPawn();
	//	if (nullptr == ControlledPawn) // 만약 아직 AI가 폰에 빙의하지 않았다면
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Orange, "FAIL : SetRandomPatrolLocation() ---> Cant Found OwnerPawn.");
	//		return;
	//	}
	//	else
	//	{
	//		FVector curPos = owner->GetActorLocation();
	//		FVector wannaPos = AI->GetWannaPos();
	//		float Dist = FVector::Dist(curPos, wannaPos);

	//		if (Dist > 10)
	//		{
	//			FVector wannaDir = (wannaPos - curPos).GetSafeNormal();
	//			FRotator curRot = owner->GetActorRotation();
	//			FRotator wannaRot = wannaDir.Rotation();

	//			FRotator newRot = FMath::RInterpTo(curRot, wannaRot, DeltaTime, rotSpeed);
	//			owner->SetActorRotation(newRot);

	//			FVector newPos = curPos + -owner->GetActorForwardVector() * speed * DeltaTime;
	//			owner->SetActorLocation(newPos);
	//		}
	//	}
	//}
}

void UMiniMonsterPatrolState::Exit_Implementation()
{
	//GetWorld()->GetTimerManager().ClearTimer(repeatTimer);
	speed = 200.f;
}