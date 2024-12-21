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

	//! Collision ����
	triggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // Overlap �̺�Ʈ�� ó���ؿ�.
	triggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic); // Static ��ü�� ����.
	triggerBox->SetCollisionResponseToAllChannels(ECR_Ignore); // ��� �浹�� �����ֱ� ���� �����.
	triggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // �׸��� Pawn�� ���� ������ ó���� �����.

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
	AMiniMonster* colActor = Cast<AMiniMonster>(OtherActor); // MiniMonster�� Trigger �ߵ�

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

