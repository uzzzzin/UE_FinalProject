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
