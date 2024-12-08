#include "MainPlayer.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "MainPlayerAnimInstance.h"

AMainPlayer::AMainPlayer()
	: bControlSpringArmYawOnly(false)
	, bIsMoving(false)
	, bIsAttacking(false)
	, bIsJumping(false)
	, bIsSiuuuuAttacking(false)
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

	//! Debug - State�� ���ϴ� ���� Ȯ��
	UMainPlayerAnimInstance* animInst = Cast<UMainPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	curState = animInst->GetCurrentStateName(0);
	if (curState != prevState)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("%s -> %s"), *prevState.ToString() ,*curState.ToString()));
		prevState = curState;
	}

	//! Debug - Attacking Ȯ�� ������ ���� ���ϴ� ���� Ȯ��
	if (prevIsAttacking != bIsAttacking)
	{
		if (bIsAttacking)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("IsAttacking : false -> true"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("IsAttacking : true -> false"));
		}
		prevIsAttacking = bIsAttacking;
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
	if (false == bIsAttacking) // Attack ���� ��, ��� ������ ������ �ؿ�.
	{
		bIsAttacking = true; // ���� ����!
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "AMainPlayer::Attack()");

		if (false == bIsMoving && GetVelocity().Size() <= 0.f) // �ӵ��� �� ���� ���� Idle ����.
		{
			if (true == bIsJumping) // �������� �ƴѵ� �������� -> State : IdleJump
			{
				bIsAttacking = false; // IdleJump�� ���� ������ ��������.
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "???");
			}
			else // �������� �ƴѵ� �����ߵ� �ƴ�. -> State : IdleAttack
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Silver, "???");
				GetCharacterMovement()->MaxWalkSpeed = 0.0f; // IdleAttack(DefaultAttack) ���ȿ��� ĳ���Ͱ� �������� ���� �ſ���.
			}
		}
		else // true == bIsMoving // State : MovetAtack or SiuuuuAttack
		{
			if (bIsJumping) // �������ε�, ������ ������? State : SiuuuuAttack
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "???");
				bIsSiuuuuAttacking = true;
			}
			else // �������� �ƴϸ� State : MoveAttack
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "???");
			}
		}
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

	if (bIsMoving && bIsAttacking) // ������, ������ -> State : MoveAttack
	{
		bIsJumping = false;
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
	//! ���⿡ ���� ������ �ȵʹ̴�
	bIsAttacking = false;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("AMainPlayer::OnMontageEndedCallback()"));
}