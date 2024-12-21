#include "MainPlayer.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Component/StateMachineComponent.h"
#include "MainPlayerAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "../UI/MainPlayerUserWidget.h"

AMainPlayer::AMainPlayer()
	: bControlSpringArmYawOnly(false)
	, firstClick(false)
	, bIsJumping(false)
	, bIsAttacking(false)
	, bIsSiuuuuAttacking(false)
	, bIsQAttacking(false)
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
	springArm->TargetArmLength = 700.f;
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

	//! Attack Montage ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage> montage(TEXT("/Script/Engine.AnimMontage'/Game/Player/AM_MainPlayer_Attack_Pole.AM_MainPlayer_Attack_Pole'"));
	if (montage.Succeeded())
	{
		AttackMontage = montage.Object;
	}

	//! UserWidgetClass�� �������Ʈ ��ο��� �ε�
	static ConstructorHelpers::FClassFinder<UMainPlayerUserWidget> WidgetBPClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_MainPlayerUI.BP_MainPlayerUI_C'"));
	if (WidgetBPClass.Class != nullptr)
	{
		WidgetClass = WidgetBPClass.Class;
	}
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

	//! Widget ����
	if (WidgetClass)
	{
		WidgetInstance = CreateWidget<UMainPlayerUserWidget>(GetWorld(), WidgetClass);
		if (WidgetInstance)
		{
			WidgetInstance->AddToViewport();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Widget Class Is not Set....");
	}
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
	PlayerInputComponent->BindAction(TEXT("JumpAttack"), EInputEvent::IE_Pressed, this, &AMainPlayer::QAttack); // �����̽���
	PlayerInputComponent->BindAction(TEXT("SiuuuuAttack"), EInputEvent::IE_Pressed, this, &AMainPlayer::EAttack); // �����̽���

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
	if (false == firstClick)
	{ 
		// �𸮾� �����Ϳ��� ���콺 ��Ŀ���� ���� ù Ŭ���� �ص� �������� �����Ǳ� ������, 
		// ���콺 ��Ŀ�� �� ù Ŭ���� ��ȿ ùŬ������ �ϱ� ���� üũ�� ����.
		firstClick = true;
		return;
	}

	if (false == bIsAttacking) // �������� �ƴ� ���� ������ �ؾ� �ؿ�.
	{
		bIsAttacking = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "void AMainPlayer::Attack()");
	}

	if (true == bIsJumping) // �������� ���� ������ ������ �ؿ�.
	{
		bIsAttacking = false;
		return;
	}
}

void AMainPlayer::MyJump()
{
	Super::Jump();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "void AMainPlayer::MyJump()");

	if (true == bIsSiuuuuAttacking)
	{
		bIsJumping = false; // Siuuuu���� ��, �����ϸ� �� �˴ϴ�.
	}
	else
	{
		bIsJumping = true;
	}
}

void AMainPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (true == bIsJumping) // ���� �����ߴٸ� ��Ǫ���� �ƴϿ���.
	{
		bIsJumping = false;
	}
	
}

void AMainPlayer::QAttack()
{
	if (false == bIsQAttacking && false == bIsJumping && false == bIsAttacking && false == bIsSiuuuuAttacking)
	{
		bIsQAttacking = true;
		WidgetInstance->HighlightQSkillImage();
	}
}

void AMainPlayer::EAttack()
{
	if (false == bIsSiuuuuAttacking && false == bIsJumping && false == bIsAttacking && false == bIsQAttacking)
	{
		bIsSiuuuuAttacking = true;
		WidgetInstance->HighlightESkillImage();
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

void AMainPlayer::PlayNormalAttackMontage()
{
	UMainPlayerAnimInstance* animInst = Cast<UMainPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	animInst->Montage_Play(AttackMontage, 1.f);
	animInst->Montage_JumpToSection(FName(*FString::Printf(TEXT("PoleAttack%d"), AttackAnimNum)), AttackMontage);
	if (AttackAnimNum >= 3)
		AttackAnimNum = 0;
	else
		AttackAnimNum += 1;
}

void AMainPlayer::OnMontageEndedCallback(UAnimMontage* Montage, bool bInterrupted)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, TEXT("AMainPlayer::OnMontageEndedCallback()"));
}