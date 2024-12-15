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
	
	GetMesh()->SetupAttachment(RootComponent); // RootComponent에 Mesh Component 부착

	//! Skeletal Mesh Asset 꽂기
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSunWukong/Characters/Heroes/Wukong/Meshes/Wukong.Wukong'"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f)); // 캡슐 세로 반경만큼 메시 이동.
		GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));
	}

	//! Spring Arm Componet 설정
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(RootComponent);
	springArm->TargetArmLength = 600.f;
	springArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
	springArm->bDoCollisionTest = false; // 충돌 시, 줌인효과 false

	//! Camera Component 설정
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("camera"));
	camera->SetupAttachment(springArm);

	//! Animation Instance 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Player/BP_MainPlayerAnimInstance.BP_MainPlayerAnimInstance_C'"));
	if (AnimBPClass.Class)
	{
		AnimInstanceBP = AnimBPClass.Class;
	}

	//! State Machine 설정
	SM = CreateDefaultSubobject<UStateMachineComponent>(TEXT("SM"));
	//SM->LoadFSMData(TEXT("/Script/Engine.DataTable'/Game/Data/MainPlayerStateMachine.MainPlayerStateMachine'"));
	SM->SetFSMDataPath(TEXT("/Script/Engine.DataTable'/Game/Data/MainPlayerStateMachine.MainPlayerStateMachine'"));

	//! 마우스로 제어하는 시야 세팅
	bUseControllerRotationPitch = true; // 무브먼트 이용해서 Pitch Rotation 가능하도록 설정.
}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	//! DefaultZOffset 세팅
	FVector CameraWorldPos = GetCameraWorldLocation();
	DefaultZOffset = GetActorLocation().Z - CameraWorldPos.Z; // 캐릭터.Z - 카메라 위치.Z

	//! DefaultSocketOffset 세팅
	DefaultSocketOffset = springArm->SocketOffset;

	//! DefaultMovementMaxWalkSpeed 세팅
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
	PlayerInputComponent->BindAxis(TEXT("MousePitch"), this, &AMainPlayer::MousePitch); // 위,아래
	PlayerInputComponent->BindAxis(TEXT("MouseYaw"), this, &AMainPlayer::MouseYaw); // 좌, 우

	// Player Keyboard Input
	PlayerInputComponent->BindAxis(TEXT("MoveVertical"), this, &AMainPlayer::MoveVertical); // W, S
	PlayerInputComponent->BindAxis(TEXT("MoveHorizontal"), this, &AMainPlayer::MoveHorizontal); // A, D 
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMainPlayer::Attack); // 마우스 좌클릭
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMainPlayer::MyJump); // 스페이스바

	// Debug Input
	// 마우스로 시점 전환할 때 LShift 누르면 플레이어의 방향벡터는 마우스 화면을 안 따라가도록 할건데 그걸 키고 끌 변수.
	PlayerInputComponent->BindAction(TEXT("ControlSpringArmYawOnly"), EInputEvent::IE_Pressed, this, &AMainPlayer::OnControlSpringArmYawOnly); // LShift
	PlayerInputComponent->BindAction(TEXT("ControlSpringArmYawOnly"), EInputEvent::IE_Released, this, &AMainPlayer::OffControlSpringArmYawOnly); // LShift
	PlayerInputComponent->BindAction(TEXT("DebugCurStateName"), EInputEvent::IE_Pressed, this, &AMainPlayer::DebugCurrentState); // O(not zero)
}

void AMainPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	 //! BP Animation Instance 연결
	if (GetMesh())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceBP);
	}

	//! Attack Montage -> OnMontageEnded (내장)델리게이트 Binding (몽타주가 끝났을 때)
	UMainPlayerAnimInstance* animInst = Cast<UMainPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	animInst->OnMontageEnded.AddDynamic(this, &AMainPlayer::OnMontageEndedCallback);

	//! StateMachine CSV 데이터 불러오기.
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
		this->AddMovementInput(this->GetActorForwardVector(), _v); // 앞, 뒤
	}
	Cast<UMainPlayerAnimInstance>(GetMesh()->GetAnimInstance())->AddMoveVertical(_v);
}

void AMainPlayer::MoveHorizontal(float _v)
{
	if (_v != 0)
	{
		this->AddMovementInput(this->GetActorRightVector(), _v); // 좌, 우
	}
	Cast<UMainPlayerAnimInstance>(GetMesh()->GetAnimInstance())->AddMoveHorizontal(_v); // BS-Jog 각도 변경
}

void AMainPlayer::MousePitch(float _v)
{
	if (_v != 0) // SpringArm의 각도만 변경됨. Player의 방향벡터는 변하지 않음.
	{
		//TODO : 게임에서는 계속 위아래로 움직이면 너무 산만해보일 것 같으니 
		//TODO : 위,아래 시점 전환 속도도 조금 늦출 필요가 있을 것 같음 !!
		FRotator armRot = springArm->GetRelativeRotation();
		armRot.Pitch = FMath::Clamp(armRot.Pitch + _v, -30.f, 5); // 각도 범위 변경
		FRotator newRot = FMath::RInterpTo(springArm->GetRelativeRotation(), armRot, GetWorld()->GetDeltaSeconds(), 20.f); // 마지막 인자는 회전 속도
		springArm->SetRelativeRotation(newRot);

		// 애니메이션 인스턴스
		Cast<UMainPlayerAnimInstance>(GetMesh()->GetAnimInstance())->AddMousePitch(_v);
	}
}

void AMainPlayer::MouseYaw(float _v)
{
	if (_v != 0)
	{
		if (true == bControlSpringArmYawOnly) // SpringArm의 각도만 변경.
		{
			FRotator armRot = springArm->GetRelativeRotation();
			armRot.Yaw = FMath::Clamp(armRot.Yaw + _v, armRot.Yaw - 70.f, armRot.Yaw + 70.f);
			FRotator newRot = FMath::RInterpTo(springArm->GetRelativeRotation(), armRot, GetWorld()->GetDeltaSeconds(), 40.f); // 마지막 인자는 회전 속도
			springArm->SetRelativeRotation(newRot);
		}
		else
		{
			this->AddControllerYawInput(_v); // Player의 방향 벡터, SpringArm의 각도 모두 변경.
		}
		Cast<UMainPlayerAnimInstance>(GetMesh()->GetAnimInstance())->AddMouseYaw(_v); // BS-InputMouse 각도 변경
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
	springArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f)); // 다시 기본 SpringArm 값으로 세팅
}

FVector AMainPlayer::GetCameraWorldLocation()
{
	//! SpringArm에 부착된 Camera의 WorldPos 구하기.
	FVector armPos = springArm->GetComponentLocation();
	FVector armDir = springArm->GetForwardVector();
	float armLength = springArm->TargetArmLength;

	FVector CameraWorldPos = armPos + (-armDir * armLength); // camera world pos 계산과정이에요.

	return CameraWorldPos;
}

void AMainPlayer::OnMontageEndedCallback(UAnimMontage* Montage, bool bInterrupted)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("AMainPlayer::OnMontageEndedCallback()"));
}