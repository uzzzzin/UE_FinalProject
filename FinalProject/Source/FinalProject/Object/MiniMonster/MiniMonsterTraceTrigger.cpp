// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniMonsterTraceTrigger.h"
#include "Components/BoxComponent.h"
#include <FinalProject/Component/StateMachineComponent.h>
#include <FinalProject/FSM/MiniMonsterFSM.h>
#include "MiniMonster.h"

// Sets default values
AMiniMonsterTraceTrigger::AMiniMonsterTraceTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	triggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));

	//! Collision 설정
	triggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // Overlap 이벤트만 처리해요.
	triggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic); // Static 객체로 설정.
	triggerBox->SetCollisionResponseToAllChannels(ECR_Ignore); // 모두 충돌을 막아주기 위해 꺼줘요.
	triggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // 그리고 Pawn에 대해 오버랩 처리만 켜줘요.

	triggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMiniMonsterTraceTrigger::OnOverlapBegin);
	triggerBox->OnComponentEndOverlap.AddDynamic(this, &AMiniMonsterTraceTrigger::OnOverlapEnd);

}

void AMiniMonsterTraceTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMiniMonsterTraceTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMiniMonsterTraceTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMiniMonster* colActor = Cast<AMiniMonster>(OtherActor); // MiniMonster만 Trigger 발동

	if(nullptr != colActor) // MiniMonste State : Patrol -> Trace
		colActor->GetComponentByClass<UStateMachineComponent>()->GetCurFSM()->ChangeState(FName("Trace"));
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, " AMiniMonsterTraceTrigger::OnOverlapBegin --> MainPlayer..");
	}
}

void AMiniMonsterTraceTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

