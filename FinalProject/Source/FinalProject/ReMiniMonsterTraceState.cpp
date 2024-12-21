// Fill out your copyright notice in the Description page of Project Settings.

#include "ReMiniMonsterTraceState.h"
#include <Kismet/GameplayStatics.h>
#include "Object/MainPlayer.h"
#include "Object/MiniMonster/MiniMonster.h"
#include "Component/StateMachineComponent.h"
#include "FSM/MiniMonsterFSM.h"

UReMiniMonsterTraceState::UReMiniMonsterTraceState()
	: speed(200.f)
	, rotSpeed(3.f)
{
}

void UReMiniMonsterTraceState::Enter()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "UMiniMonsterTraceState::Enter_Implementation()");

	// 타겟 MainPlayer를 가져와요.
	target = Cast<AMainPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void UReMiniMonsterTraceState::Update(float DeltaTime)
{
	AMiniMonster* owner = Cast<AMiniMonster>(GetOwnerFSM()->GetOwnerStateMachine()->GetOwner());

	//! 타겟을 향해 방향을 틀고 쫓아가요.
	if (nullptr != target)
	{
		FVector curPos = owner->GetActorLocation();
		FVector wannaPos = target->GetActorLocation();

		float Dist = FVector::Dist(curPos, wannaPos);
		if (Dist > 10)
		{
			FVector wannaDir = (wannaPos - curPos).GetSafeNormal() * -1.f;
			FRotator curRot = owner->GetActorRotation();
			FRotator wannaRot = wannaDir.Rotation();

			FRotator newRot = FMath::RInterpTo(curRot, wannaRot, DeltaTime, rotSpeed);
			owner->SetActorRotation(newRot);
			FVector newPos = curPos + -wannaDir * speed * DeltaTime;
			owner->SetActorLocation(newPos);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "MainPlayer Is Not Found.....");
	}
}

void UReMiniMonsterTraceState::Exit()
{
}

