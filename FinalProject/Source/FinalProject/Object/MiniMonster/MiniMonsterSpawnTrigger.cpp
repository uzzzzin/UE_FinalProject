// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniMonsterSpawnTrigger.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

AMiniMonsterSpawnTrigger::AMiniMonsterSpawnTrigger()
{
 	PrimaryActorTick.bCanEverTick = true;

	triggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	
	//! Collision 설정
	triggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // Overlap 이벤트만 처리해요.
	triggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic); // Static 객체로 설정.
	triggerBox->SetCollisionResponseToAllChannels(ECR_Ignore); // 모두 충돌을 막아주기 위해 꺼줘요.
	triggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // 그리고 Pawn에 대해 오버랩 처리만 켜줘요.


	//! Overlap Delegate 설정
	triggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMiniMonsterSpawnTrigger::OnOverlapBegin);
	triggerBox->OnComponentEndOverlap.AddDynamic(this, &AMiniMonsterSpawnTrigger::OnOverlapEnd);
}

void AMiniMonsterSpawnTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMiniMonsterSpawnTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMiniMonsterSpawnTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "AMiniMonsterSpawnTrigger::OnOverlapBegin()");
}

void AMiniMonsterSpawnTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "AMiniMonsterSpawnTrigger::OnOverlapEnd()");
}