#include "MainPlayer.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Component/StateMachineComponent.h"
#include "MainPlayerAnimInstance.h"

AMainPlayer::AMainPlayer()
	: bControlSpringArmYawOnly(false)
	, bIsJumping(false)
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

	//! Animation Instance ����
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Player/BP_MainPlayerAnimInstance.BP_MainPlayerAnimInstance_C'"));
	if (AnimBPClass.Class)
	{
		AnimInstanceBP = AnimBPClass.Class;
	}

	//! State Machine ����
	SM = CreateDefaultSubobject<UStateMachineComponent>(TEXT("SM"));
	//SM->LoadFSMData(TEXT("/Script/Engine.DataTable'/Game/Data/MainPlayerStateMachine.MainPlayerStateMachine'"));
	SM->SetFSMDataPath(TEXT("/Script/Engine.DataTable'/Game/Data/MainPlayerStateMachine.MainPlayerStateMachine'"));

	//! ���콺�� �����ϴ� �þ� ����
	bUseControllerRotationPitch = true; // �����Ʈ �̿��ؼ� Pitch Rotation �����ϵ��� ����.
}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	//! DefaultZOffset ����
	FVector CameraWorldPos = GetCameraWorldLocation();
	DefaultZOffset = GetActorLocation().Z - CameraWorldPos.Z; // ĳ����.Z - ī�޶� ��ġ.Z

	//! DefaultSocketOffset ����
	DefaultSocketOffset = springArm->SocketOffset;

	//! DefaultMovementMaxWalkSpeed ����
	DefaultMovementMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Player Mouse Input
	PlayerInputComponent->BindAxis(TEXT("MousePitch"), this, &AMainPlayer::MousePitch); // ��,�Ʒ�
	PlayerInputComponent->BindAxis(TEXT("MouseYaw"), this, &AMainPlayer::MouseYaw); // ��, ��

	// Player Keyboard Input
	PlayerInputComponent->BindAxis(TEXT("MoveVertical"), this, &AMainPlayer::MoveVertical); // W, S
	PlayerInputComponent->BindAxis(TEXT("MoveHorizontal"), this, &AMainPlayer::MoveHorizontal); // A, D 
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMainPlayer::Attack); // ���콺 ��Ŭ��
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMainPlayer::MyJump); // �����̽���

	// Debug Input
	// ���콺�� ���� ��ȯ�� �� LShift ������ �÷��̾��� ���⺤�ʹ� ���콺 ȭ���� �� ���󰡵��� �Ұǵ� �װ� Ű�� �� ����.
	PlayerInputComponent->BindAction(TEXT("ControlSpringArmYawOnly"), EInputEvent::IE_Pressed, this, &AMainPlayer::OnControlSpringArmYawOnly); // LShift
	PlayerInputComponent->BindAction(TEXT("ControlSpringArmYawOnly"), EInputEvent::IE_Released, this, &AMainPlayer::OffControlSpringArmYawOnly); // LShift
	PlayerInputComponent->BindAction(TEXT("DebugCurStateName"), EInputEvent::IE_Pressed, this, &AMainPlayer::DebugCurrentState); // O(not zero)
}

void AMainPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	 //! BP Animation Instance ����
	if (GetMesh())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceBP);
	}

	//! Attack Montage -> OnMontageEnded (����)��������Ʈ Binding (��Ÿ�ְ� ������ ��)
	UMainPlayerAnimInstance* animInst = Cast<UMainPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	animInst->OnMontageEnded.AddDynamic(this, &AMainPlayer::OnMontageEndedCallback);

	//! StateMachine CSV ������ �ҷ�����.
	SM->LoadFSMData(TEXT("/Script/Engine.DataTable'/Game/Data/MainPlayerStateMachine.MainPlayerStateMachine'"));
	
	if (nullptr == SM->GetFSMData())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("( FAIL : PostInitializeComponents() FSMData is Nullptr. )"));
	}
	else
	{
		//SM->SetCurFSM(SM->GetFSMKeys()[0]);
		//SM->GetCurFSM()->SetCurStateName(SM->GetCurFSM()->GetStateKeys()[0]);
	}
}

void AMainPlayer::MoveVertical(float _v)
{
	if (_v != 0)
	{
		this->AddMovementInput(this->GetActorForwardVector(), _v); // ��, ��
	}
	Cast<UMainPlayerAnimInstance>(GetMesh()->GetAnimInstance())->AddMoveVertical(_v);
}

void AMainPlayer::MoveHorizontal(float _v)
{
	if (_v != 0)
	{
		this->AddMovementInput(this->GetActorRightVector(), _v); // ��, ��
	}
	Cast<UMainPlayerAnimInstance>(GetMesh()->GetAnimInstance())->AddMoveHorizontal(_v); // BS-Jog ���� ����
}

void AMainPlayer::MousePitch(float _v)
{
	if (_v != 0) // SpringArm�� ������ �����. Player�� ���⺤�ʹ� ������ ����.
	{
		//TODO : ���ӿ����� ��� ���Ʒ��� �����̸� �ʹ� �길�غ��� �� ������ 
		//TODO : ��,�Ʒ� ���� ��ȯ �ӵ��� ���� ���� �ʿ䰡 ���� �� ���� !!
		FRotator armRot = springArm->GetRelativeRotation();
		armRot.Pitch = FMath::Clamp(armRot.Pitch + _v, -30.f, 5); // ���� ���� ����
		FRotator newRot = FMath::RInterpTo(springArm->GetRelativeRotation(), armRot, GetWorld()->GetDeltaSeconds(), 20.f); // ������ ���ڴ� ȸ�� �ӵ�
		springArm->SetRelativeRotation(newRot);

		// �ִϸ��̼� �ν��Ͻ�
		Cast<UMainPlayerAnimInstance>(GetMesh()->GetAnimInstance())->AddMousePitch(_v);
	}
}

void AMainPlayer::MouseYaw(float _v)
{
	if (_v != 0)
	{
		if (true == bControlSpringArmYawOnly) // SpringArm�� ������ ����.
		{
			FRotator armRot = springArm->GetRelativeRotation();
			armRot.Yaw = FMath::Clamp(armRot.Yaw + _v, armRot.Yaw - 70.f, armRot.Yaw + 70.f);
			FRotator newRot = FMath::RInterpTo(springArm->GetRelativeRotation(), armRot, GetWorld()->GetDeltaSeconds(), 40.f); // ������ ���ڴ� ȸ�� �ӵ�
			springArm->SetRelativeRotation(newRot);
		}
		else
		{
			this->AddControllerYawInput(_v); // Player�� ���� ����, SpringArm�� ���� ��� ����.
		}
		Cast<UMainPlayerAnimInstance>(GetMesh()->GetAnimInstance())->AddMouseYaw(_v); // BS-InputMouse ���� ����
	}
}

void AMainPlayer::Attack()
{
}

void AMainPlayer::MyJump()
{
	Super::Jump();
	bIsJumping = true;
}

void AMainPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (true == bIsJumping)
	{
		bIsJumping = false;
	}
	
}

void AMainPlayer::DebugCurrentState()
{
	UMainPlayerAnimInstance* animInst = Cast<UMainPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	FName CurStateName = animInst->GetCurrentStateName(0);

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, FString::Printf(TEXT("Current State Name: %s"), *CurStateName.ToString()));
}

void AMainPlayer::SetMovementMaxWalkSpeed(float _v)
{
	GetCharacterMovement()->MaxWalkSpeed = _v;
}

void AMainPlayer::OffControlSpringArmYawOnly()
{
	bControlSpringArmYawOnly = false;
	springArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f)); // �ٽ� �⺻ SpringArm ������ ����
}

FVector AMainPlayer::GetCameraWorldLocation()
{
	//! SpringArm�� ������ Camera�� WorldPos ���ϱ�.
	FVector armPos = springArm->GetComponentLocation();
	FVector armDir = springArm->GetForwardVector();
	float armLength = springArm->TargetArmLength;

	FVector CameraWorldPos = armPos + (-armDir * armLength); // camera world pos �������̿���.

	return CameraWorldPos;
}

void AMainPlayer::OnMontageEndedCallback(UAnimMontage* Montage, bool bInterrupted)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("AMainPlayer::OnMontageEndedCallback()"));
}