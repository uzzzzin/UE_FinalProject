#include "MainPlayer.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMainPlayer::AMainPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetupAttachment(RootComponent); // RootComponent�� Mesh Component ����

	//! Skeletal Mesh Asset �ȱ�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSunWukong/Characters/Heroes/Wukong/Meshes/Wukong.Wukong'"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f)); // ĸ�� ���� �ݰ游ŭ �޽� �̵�.
		GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));
	}

	//! Spring Arm Componet ����
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(RootComponent);
	springArm->TargetArmLength = 600.f;
	springArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
	springArm->bDoCollisionTest = false; // �浹 ��, ����ȿ�� false

	//! Camera Component ����
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("camera"));
	camera->SetupAttachment(springArm);


	bUseControllerRotationPitch = true; // �����Ʈ �̿��ؼ� Pitch Rotation �����ϵ��� ����.
}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("(%f, %f, %f)"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z));
}

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveHorizontal"), this, &AMainPlayer::MoveHorizontal); // A, D 
	PlayerInputComponent->BindAxis(TEXT("MoveVertical"), this, &AMainPlayer::MoveVertical); // W, S

	PlayerInputComponent->BindAxis(TEXT("MousePitch"), this, &AMainPlayer::MousePitch); // ��,�Ʒ�
	PlayerInputComponent->BindAxis(TEXT("MouseYaw"), this, &AMainPlayer::MouseYaw); // ��, ��
	
	//PlayerInputComponent->BindAction(TEXT("FixThisMovement"), EInputEvent::IE_Pressed, this, &AMainPlayer::FixMovementFor); // �÷��̾� ���� ���� ����
	//PlayerInputComponent->BindAction(TEXT("FixThisMovement"), EInputEvent::IE_Released, this, &AMainPlayer::FreeMovement); // �� Shift
	// ���콺�� ���� ��ȯ�� �� ��ư�� ������ �������� ���콺 ȭ���� �� ���󰡵��� �Ұǵ� �װ� Ű�� �� �Լ� ��õ���� Ű�� �Ŷ� ���� �Ŷ� �и� ��
}

void AMainPlayer::MoveVertical(float _v)
{
	if (_v != 0)
	{
		this->AddMovementInput(this->GetActorForwardVector(), _v); // ��, ��
	}
}

void AMainPlayer::MoveHorizontal(float _v)
{
	if (_v != 0)
	{
		this->AddMovementInput(this->GetActorRightVector(), _v); // ��, ��
	}
}

void AMainPlayer::MousePitch(float _v)
{
	if (_v != 0) // SpringArm�� ������ �����. Player�� ���⺤�ʹ� ������ ����.
	{
		FRotator armRot = springArm->GetRelativeRotation();
		armRot.Pitch = FMath::Clamp(armRot.Pitch + _v, -30.f, 5); // ���� ���� ����
		FRotator newRot = FMath::RInterpTo(springArm->GetRelativeRotation(), armRot, GetWorld()->GetDeltaSeconds(), 20.f); // ������ ���ڴ� ȸ�� �ӵ�
		springArm->SetRelativeRotation(newRot);
 
		//TODO : ���ӿ����� ��� ���Ʒ��� �����̸� �ʹ� �길�غ��� �� ������ 
		//TODO : ��,�Ʒ� ���� ��ȯ �ӵ��� ���� ���� �ʿ䰡 ���� �� ����.
	}
}

void AMainPlayer::MouseYaw(float _v)
{
	if (_v != 0)
	{
		this->AddControllerYawInput(_v); // Player�� ���� ����, SpringArm�� ���� ��� ����.
	}
}
