// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MiniMonsterSpawnTrigger.generated.h"

UCLASS()
class FINALPROJECT_API AMiniMonsterSpawnTrigger : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* triggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	int spawnCount; // 최대 스폰 가능한 수

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	TArray<class AMiniMonster*> spawnedArr; // 스폰된 애들의 배열

	UPROPERTY(EditAnywhere, Category = "Spawning")
	FString spawnTargetName; // 스폰 기준이 되는 액터의 이름

public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	AMiniMonsterSpawnTrigger();

};
