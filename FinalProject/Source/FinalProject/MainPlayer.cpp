#include "MainPlayer.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMainPlayer::AMainPlayer()
	: bControlSpringArmYawOnly(false)
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


	bUseControllerRotationPitch = true; // 무브먼트 이용해서 Pitch Rotation 가능하도록 설정.
}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveHorizontal"), this, &AMainPlayer::MoveHorizontal); // A, D 
	PlayerInputComponent->BindAxis(TEXT("MoveVertical"), this, &AMainPlayer::MoveVertical); // W, S

	PlayerInputComponent->BindAxis(TEXT("MousePitch"), this, &AMainPlayer::MousePitch); // 위,아래
	PlayerInputComponent->BindAxis(TEXT("MouseYaw"), this, &AMainPlayer::MouseYaw); // 좌, 우
	
	// 마우스로 시점 전환할 때 LShift 누르면 플레이어의 방향벡터는 마우스 화면을 안 따라가도록 할건데 그걸 키고 끌 변수.
	PlayerInputComponent->BindAction(TEXT("ControlSpringArmYawOnly"), EInputEvent::IE_Pressed, this, &AMainPlayer::OnControlSpringArmYawOnly);
	PlayerInputComponent->BindAction(TEXT("ControlSpringArmYawOnly"), EInputEvent::IE_Released, this, &AMainPlayer::OffControlSpringArmYawOnly);
}

void AMainPlayer::MoveVertical(float _v)
{
	if (_v != 0)
	{
		this->AddMovementInput(this->GetActorForwardVector(), _v); // 앞, 뒤
	}
}

void AMainPlayer::MoveHorizontal(float _v)
{
	if (_v != 0)
	{
		this->AddMovementInput(this->GetActorRightVector(), _v); // 좌, 우
	}
}

void AMainPlayer::MousePitch(float _v)
{
	if (_v != 0) // SpringArm의 각도만 변경됨. Player의 방향벡터는 변하지 않음.
	{
		FRotator armRot = springArm->GetRelativeRotation();
		armRot.Pitch = FMath::Clamp(armRot.Pitch + _v, -30.f, 5); // 각도 범위 변경
		FRotator newRot = FMath::RInterpTo(springArm->GetRelativeRotation(), armRot, GetWorld()->GetDeltaSeconds(), 20.f); // 마지막 인자는 회전 속도
		springArm->SetRelativeRotation(newRot);
 
		//TODO : 게임에서는 계속 위아래로 움직이면 너무 산만해보일 것 같으니 
		//TODO : 위,아래 시점 전환 속도도 조금 늦출 필요가 있을 것 같음.
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
			FRotator newRot = FMath::RInterpTo(springArm->GetRelativeRotation(), armRot, GetWorld()->GetDeltaSeconds(), 20.f); // 마지막 인자는 회전 속도
			springArm->SetRelativeRotation(newRot);
		}
		else
		{
			this->AddControllerYawInput(_v); // Player의 방향 벡터, SpringArm의 각도 모두 변경.
		}
	}
}

void AMainPlayer::OffControlSpringArmYawOnly()
{
	bControlSpringArmYawOnly = false;
	springArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f)); // 다시 기본 SpringArm 값으로 세팅
}
