// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniMonsterSpawnTrigger.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "MiniMonster.h"

AMiniMonsterSpawnTrigger::AMiniMonsterSpawnTrigger()
	: spawnCount(30)
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

    //! SpawnTarget Name ����
    spawnTargetName = FName(TEXT("SM_door_03")).ToString();
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

    int curSpawnCount = 0;

    while (curSpawnCount <= spawnCount)
    {
        FVector spawnPos;
        FRotator spawnRot;
        int randomNum = FMath::RandRange(0, 1); // 0�̸� L, 1�̸� R

        if (0 == randomNum)
        {
            spawnPos = FVector(20200.f, 2550.f, 1600.f);
            spawnRot = FRotator(0, -90, 0.f);
        }
        else
        {
            spawnPos = FVector(19740.f, 7830.f, 1600.f);
            spawnRot = FRotator(0, 90, 0.f);
        }

        // ���� ��ġ �� ȸ��: Ʈ���� ������ ��ġ�� ȸ���� �������� ����
        FVector randPos = FVector(FMath::FRandRange(-500.f, 500.f), FMath::FRandRange(-500.f, 500.f), 0.f);
        FVector SpawnLocation = spawnPos + randPos; // �迭 ũ�⿡ ���� Y�� ������
        FRotator SpawnRotation = GetActorRotation() + spawnRot;

        // Pawn ����
        AMiniMonster* miniMonster = GetWorld()->SpawnActor<AMiniMonster>(AMiniMonster::StaticClass(), SpawnLocation, SpawnRotation);

        if (miniMonster)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Pawn spawned successfully at %s."), *SpawnLocation.ToString()));
            spawnedArr.Add(miniMonster); // �迭�� �߰�
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Failed to spawn Pawn.");
        }

        ++curSpawnCount;
    }

}

void AMiniMonsterSpawnTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "AMiniMonsterSpawnTrigger::OnOverlapEnd()");
}