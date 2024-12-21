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
	
	//! Collision 설정
	triggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // Overlap 이벤트만 처리해요.
	triggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic); // Static 객체로 설정.
	triggerBox->SetCollisionResponseToAllChannels(ECR_Ignore); // 모두 충돌을 막아주기 위해 꺼줘요.
	triggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // 그리고 Pawn에 대해 오버랩 처리만 켜줘요.


	//! Overlap Delegate 설정
	triggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMiniMonsterSpawnTrigger::OnOverlapBegin);
	triggerBox->OnComponentEndOverlap.AddDynamic(this, &AMiniMonsterSpawnTrigger::OnOverlapEnd);

    //! SpawnTarget Name 설정
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
        int randomNum = FMath::RandRange(0, 1); // 0이면 L, 1이면 R

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

        // 스폰 위치 및 회전: 트리거 액터의 위치와 회전을 기준으로 설정
        FVector randPos = FVector(FMath::FRandRange(-500.f, 500.f), FMath::FRandRange(-500.f, 500.f), 0.f);
        FVector SpawnLocation = spawnPos + randPos; // 배열 크기에 따라 Y축 오프셋
        FRotator SpawnRotation = GetActorRotation() + spawnRot;

        // Pawn 스폰
        AMiniMonster* miniMonster = GetWorld()->SpawnActor<AMiniMonster>(AMiniMonster::StaticClass(), SpawnLocation, SpawnRotation);

        if (miniMonster)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Pawn spawned successfully at %s."), *SpawnLocation.ToString()));
            spawnedArr.Add(miniMonster); // 배열에 추가
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