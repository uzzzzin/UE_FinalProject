#include "MainPlayer.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "MainPlayerAnimInstance.h"

AMainPlayer::AMainPlayer()
	: bControlSpringArmYawOnly(false)
	, bIsMoving(false)
	, AttackComboNum(0)
	, bIsAttacking(false)
	, AttackMontagePlayRate(2.f)
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

	//! Attack Montage ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage> montage(TEXT("/Script/Engine.AnimMontage'/Game/Player/AM_MainPlayer_Attack_Pole.AM_MainPlayer_Attack_Pole'"));
	if (montage.Succeeded())
	{
		AttackMontage = montage.Object;
	}

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
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// ĳ���� �̵������� Ȯ��, AnimInstance���� State���� �� ���.
	bIsMoving = (GetVelocity().Size() > 0.f) && (GetCharacterMovement()->IsMovingOnGround());

	//! ���� ���¿���, ī�޶��� Z�� �������� ���� ĳ������ Z�� ����ȭ.
	if (bIsJumping)
	{
		//! ���� ���¿��� ī�޶� ���� Ƣ�� �;��.
		FVector curCameraPos = GetCameraWorldLocation();
		float curZDiff = GetActorLocation().Z - curCameraPos.Z; // ���� ĳ����.Z - ī�޶� ��ġ.Z

		if (curZDiff != DefaultZOffset) // �⺻ Z Offset�̶� ���� Z Offset�� �ٸ��ٸ� --> Z���� ���̰� ������ (�翬�� ���� ���´� ����.)
		{
			// Camera�� ��ġ�� Socket Offset�� ���� �����ؿ�.
			// �����ϰ�, �����ϴ� �������� ���� Ƣ�� ī�޶� ������ �ְ� �;����.
			FVector wantSocketOffset = FVector(0, 0, DefaultZOffset - curZDiff);
			springArm->SocketOffset = FMath::VInterpTo(springArm->SocketOffset, wantSocketOffset, DeltaTime, 3.5f); // 3.5�� ���� ��︮�� ��.
			//TODO: Camera�� ��ġ�� ������ �� ����, �÷����� �ո��� �� �ƴ��� Ȯ���ϸ鼭 �������� ������ �ɾ�� �� �� ���ƿ�.
		}
	}
	else
	{
		// Spring Arm -> Socket Offset �ٽ� ������ DEfault ������ ����
		springArm->SocketOffset = FMath::VInterpTo(springArm->SocketOffset, DefaultSocketOffset, DeltaTime, 3.5f);

	}
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
}

void AMainPlayer::MoveVertical(float _v)
{
	if (_v != 0)
	{
		this->AddMovementInput(this->GetActorForwardVector(), _v); // ��, ��
		bIsMoving = true; // Ű�� ������ ���� �� �ӷ��� ������ ������ ���·� �����ϰ� ����.
	}
	Cast<UMainPlayerAnimInstance>(GetMesh()->GetAnimInstance())->AddMoveVertical(_v);
}

void AMainPlayer::MoveHorizontal(float _v)
{
	if (_v != 0)
	{
		this->AddMovementInput(this->GetActorRightVector(), _v); // ��, ��
		bIsMoving = true; // Ű�� ������ ���� ��, �ӷ��� ������ ������ ���·� �����ϰ� ����.
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
	bIsAttacking = true; // ���� ����!

	if (false == bIsMoving) // State :  IdleAttack
	{
		PlayAttackMontage();
	}
	else // true == bIsMoving // State : MoveAttack
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "true == bIsMoving");
	}
}

void AMainPlayer::MyJump()
{
	UMainPlayerAnimInstance* animInst = Cast<UMainPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	FName CurStateName = animInst->GetCurrentStateName(0);

	if (!bIsJumping && GetCharacterMovement()->IsMovingOnGround())
	{
		if (bIsMoving) // MoveJump State
		{
			bIsJumping = true; 
		}
		else // false == IsMoving, IdleJump State
		{
			bIsJumping = true; 
		}
	}
}

void AMainPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	// ���� ���¿��� ��, ���� ������ ���� ���°� ������.
	if (bIsJumping)
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

void AMainPlayer::PlayAttackMontage() // Attack Ű ���Ⱦ��
{
	UMainPlayerAnimInstance* animInst = Cast<UMainPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	
	if (!animInst->Montage_IsPlaying(AttackMontage)) // ���� �ִϸ��̼��� ���������� ���� ����.
	{
		if (true == animInst->GetbAttackEnded()) // recovery ���� Ȥ�� �ƹ� ���°� �ƴ� --> ���� ��ȿ
		{
			animInst->Montage_Play(AttackMontage, AttackMontagePlayRate);
			animInst->SetbAttackEnded(false);
			// �Ź� �ٸ� �ִϸ��̼��� ����� �� �ֵ���.
			animInst->Montage_JumpToSection(FName(*FString::Printf(TEXT("PoleAttack%d"), AttackComboNum)), AttackMontage);
			if (AttackComboNum >= 3)
				AttackComboNum = 0;
			else
				AttackComboNum += 1;
		}
		// else -> �ƹ� ���µ� �ƴϿ���, ���� ��찡 ���� ���·� ����. (recovery ���°� �ƴϿ���.) --> ������ ������.
	}
	else // ���� �ִϸ��̼��� �������� ��, == ���� Ű�� ��Ÿ�ؼ� �Լ��� ���� ������.
	{
		if (true == animInst->GetbAttackEnded()) // ���� ���� �ִϸ��̼��� recovery ���� --> ���� ��ȿ !
		{
			//TODO �ִϸ��̼� ���� �ӵ��� �ʹ� �������� �ִϸ��̼��� ������. �����ؼ� �� ����.
			animInst->Montage_SetPlayRate(AttackMontage, AttackMontagePlayRate * 1.3f); // Recovery �ִϸ��̼� �ӵ��� ������ �ؼ� �� �ִϸ��̼��� ��������. 
			animInst->Montage_Play(AttackMontage, AttackMontagePlayRate);
			animInst->SetbAttackEnded(false);
			// �Ź� �ٸ� �ִϸ��̼��� ����� �� �ֵ���.
			animInst->Montage_JumpToSection(FName(*FString::Printf(TEXT("PoleAttack%d"), AttackComboNum)), AttackMontage);
			if (AttackComboNum >= 3)
				AttackComboNum = 0;
			else
				AttackComboNum += 1;
		}
		// else // false == animInst->GetbAttackEnded() // ���� ���� ���� ���� (recovery ���´� ���� ���� �ʾƵ�..) --> ������ ������.
	}
}

void AMainPlayer::OnMontageEndedCallback(UAnimMontage* Montage, bool bInterrupted)
{
	//! AttackMontage�� ���� ��Ÿ�� ������ ������ ��,
	//! �������� ���� = false;
	//! IdleAttack�� ȣȯ��. 
	//! MoveAttack���� ȣȯ���� ����.
	bIsAttacking = false;
}