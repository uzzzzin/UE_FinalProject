// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniMonster.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "../../Component/StateMachineComponent.h"
#include "MiniMonsterAIController.h"

// Sets default values
AMiniMonster::AMiniMonster()
	: wolfStyle(0)
{
 	PrimaryActorTick.bCanEverTick = true;

	//! Capsule Component �� ����
	GetCapsuleComponent()->SetCapsuleSize(100.f, 88.f); // ĸ�� �ݰ� ����.

	//! Skeletal Mesh Asset ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ForestAnimalsPack/Wolf/Meshes/SK_Wolf.SK_Wolf'"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f)); // ĸ�� ���� �ݰ游ŭ �޽� �̵�.
		GetMesh()->SetRelativeRotation(FRotator(0, 90.f, 0.f));
		GetMesh()->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
	}

	//! Animation Instance ����
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Monster/BP_MiniMonsterAnimInstance.BP_MiniMonsterAnimInstance_C'"));
	if (AnimBPClass.Class)
	{
		AnimInstanceBP = AnimBPClass.Class;
	}

	//! AI Controller ����
	AIControllerClass = AMiniMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; // �� Ŭ���� ���� ���ʹ� ������ �� �����ϰھ��.

	//! State Machine ����
	SM = CreateDefaultSubobject<UStateMachineComponent>(TEXT("SM"));
	SM->SetFSMDataPath(TEXT("/Script/Engine.DataTable'/Game/Data/MiniMonsterStateMachine.MiniMonsterStateMachine'"));

}

void AMiniMonster::SetRandomMaterial()
{
	if (nullptr != GetMesh())
	{
		// �������� ���ϴ� ���͸����� �����ؼ� �޽ÿ� �Ⱦ����.
		FString path0 = TEXT("/Script/Engine.Material'/Game/ForestAnimalsPack/Wolf/Materials/M_Wolf.M_Wolf'");
		FString path1 = TEXT("/Script/Engine.Material'/Game/ForestAnimalsPack/Wolf/Materials/M_WolfFur.M_WolfFur'");
		
		// wolf Style�� ���� ���͸����� �ٸ��� ���õǾ��.
		switch (wolfStyle)
		{
			case 0:
			{
				path0 = TEXT("/Script/Engine.Material'/Game/ForestAnimalsPack/Wolf/Materials/M_Wolf.M_Wolf'");
				path1 = TEXT("/Script/Engine.Material'/Game/ForestAnimalsPack/Wolf/Materials/M_WolfFur.M_WolfFur'");
				break;
			}
			case 1:
			{
				path0 = TEXT("/Script/Engine.Material'/Game/ForestAnimalsPack/Wolf/Materials/M_WolfArctic.M_WolfArctic'");
				path1 = TEXT("/Script/Engine.Material'/Game/ForestAnimalsPack/Wolf/Materials/M_WolfFurArctic.M_WolfFurArctic'");
				break;
			}
			case 2:
			{
				path0 = TEXT("/Script/Engine.Material'/Game/ForestAnimalsPack/Wolf/Materials/M_WolfDark.M_WolfDark'");
				path1 = TEXT("/Script/Engine.Material'/Game/ForestAnimalsPack/Wolf/Materials/M_WolfFurDark.M_WolfFurDark'");
				break;
			}
			default:
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "AMiniMonster::SetRandomMaterial() ---> set default Material is.");
				break;
			}
		}

		UMaterialInterface* Mtrl0 = LoadObject<UMaterialInterface>(nullptr, *path0); // �޾ƿ� ��ο� �ش��ϴ� ���͸��� �ε��ؿ�.
		UMaterialInterface* Mtrl1 = LoadObject<UMaterialInterface>(nullptr, *path1);

		if (nullptr != Mtrl0 && nullptr != Mtrl1)
		{
			GetMesh()->SetMaterial(0, Mtrl0); // �޽� ���� 0���� ��Ƽ���� �Ⱦ����.
			GetMesh()->SetMaterial(1, Mtrl1); // �޽� ���� 1����.
		}
	}
}

void AMiniMonster::BeginPlay()
{
	Super::BeginPlay();
	
	wolfStyle = FMath::RandRange(0, 2);
	SetRandomMaterial();
}

void AMiniMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMiniMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMiniMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//! BP Animation Instance ����
	if (GetMesh())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceBP);
	}

	//! StateMachine CSV ������ �ҷ�����.
	SM->LoadFSMData(TEXT("/Script/Engine.DataTable'/Game/Data/MiniMonsterStateMachine.MiniMonsterStateMachine'"));

	if (nullptr == SM->GetFSMData())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("( FAIL : PostInitializeComponents() FSMData is Nullptr. )"));
	}
}

