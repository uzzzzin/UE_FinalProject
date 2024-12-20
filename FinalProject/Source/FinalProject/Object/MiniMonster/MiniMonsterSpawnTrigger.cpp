// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniMonsterSpawnTrigger.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

AMiniMonsterSpawnTrigger::AMiniMonsterSpawnTrigger()
{
 	PrimaryActorTick.bCanEverTick = true;

	triggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	
	//! Collision ����
	triggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // Overlap �̺�Ʈ�� ó���ؿ�.
	triggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic); // Static ��ü�� ����.
	triggerBox->SetCollisionResponseToAllChannels(ECR_Ignore); // ��� �浹�� �����ֱ� ���� �����.
	triggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // �׸��� Pawn�� ���� ������ ó���� �����.


	//! Overlap Delegate ����
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