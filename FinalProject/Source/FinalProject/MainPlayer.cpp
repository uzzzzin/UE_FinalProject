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
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// ĳ���� �̵������� Ȯ��, AnimInstance���� State���� �� ���.
	bIsMoving = (GetVelocity().Size() > 0.f) && (GetCharacterMovement()->IsMovingOnGround());
}

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveHorizontal"), this, &AMainPlayer::MoveHorizontal); // A, D 
	PlayerInputComponent->BindAxis(TEXT("MoveVertical"), this, &AMainPlayer::MoveVertical); // W, S

	PlayerInputComponent->BindAxis(TEXT("MousePitch"), this, &AMainPlayer::MousePitch); // ��,�Ʒ�
	PlayerInputComponent->BindAxis(TEXT("MouseYaw"), this, &AMainPlayer::MouseYaw); // ��, ��
	
	// ���콺�� ���� ��ȯ�� �� LShift ������ �÷��̾��� ���⺤�ʹ� ���콺 ȭ���� �� ���󰡵��� �Ұǵ� �װ� Ű�� �� ����.
	PlayerInputComponent->BindAction(TEXT("ControlSpringArmYawOnly"), EInputEvent::IE_Pressed, this, &AMainPlayer::OnControlSpringArmYawOnly);
	PlayerInputComponent->BindAction(TEXT("ControlSpringArmYawOnly"), EInputEvent::IE_Released, this, &AMainPlayer::OffControlSpringArmYawOnly);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMainPlayer::Attack);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMainPlayer::MyJump);
}

void AMainPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
	 //! BP Animation Instance ����
	if (GetMesh())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceBP);
	}

	//! Attack Montage -> OnMontageEnded (����)��������Ʈ ���ε�
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
	PlayAttackMontage();
}

void AMainPlayer::MyJump()
{
	bIsJumping = true;
}

void AMainPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	// ���� ������� ���� ���°� �ƴϿ���.
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "AMainPlayer::Landed() << override");
	bIsJumping = false;
}

void AMainPlayer::OffControlSpringArmYawOnly()
{
	bControlSpringArmYawOnly = false;
	springArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f)); // �ٽ� �⺻ SpringArm ������ ����
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
	bIsAttacking = false;
}