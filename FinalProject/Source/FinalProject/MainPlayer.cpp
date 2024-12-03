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

	//! Attack Montage 세팅
	static ConstructorHelpers::FObjectFinder<UAnimMontage> montage(TEXT("/Script/Engine.AnimMontage'/Game/Player/AM_MainPlayer_Attack_Pole.AM_MainPlayer_Attack_Pole'"));
	if (montage.Succeeded())
	{
		AttackMontage = montage.Object;
	}

	//! 마우스로 제어하는 시야 세팅
	bUseControllerRotationPitch = true; // 무브먼트 이용해서 Pitch Rotation 가능하도록 설정.
}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 캐릭터 이동중인지 확인, AnimInstance에서 State변경 시 사용.
	bIsMoving = (GetVelocity().Size() > 0.f) && (GetCharacterMovement()->IsMovingOnGround());
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
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMainPlayer::Attack);
}

void AMainPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
	 //! BP Animation Instance 연결
	if (GetMesh())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceBP);
	}

	//! Attack Montage -> OnMontageEnded (내장)델리게이트 바인딩
	UMainPlayerAnimInstance* animInst = Cast<UMainPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	animInst->OnMontageEnded.AddDynamic(this, &AMainPlayer::OnMontageEndedCallback);
}

void AMainPlayer::MoveVertical(float _v)
{
	if (_v != 0)
	{
		this->AddMovementInput(this->GetActorForwardVector(), _v); // 앞, 뒤
		bIsMoving = true; // 키를 누르고 있을 때 속력이 없더라도 움직임 상태로 유지하고 싶음.
	}
	Cast<UMainPlayerAnimInstance>(GetMesh()->GetAnimInstance())->AddMoveVertical(_v);
}

void AMainPlayer::MoveHorizontal(float _v)
{
	if (_v != 0)
	{
		this->AddMovementInput(this->GetActorRightVector(), _v); // 좌, 우
		bIsMoving = true; // 키를 누르고 있을 때, 속력이 없더라도 움직임 상태로 유지하고 싶음.
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
	bIsAttacking = true; // 공격 상태!
	PlayAttackMontage();
}

void AMainPlayer::OffControlSpringArmYawOnly()
{
	bControlSpringArmYawOnly = false;
	springArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f)); // 다시 기본 SpringArm 값으로 세팅
}

void AMainPlayer::PlayAttackMontage() // Attack 키 눌렸어요
{
	UMainPlayerAnimInstance* animInst = Cast<UMainPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	
	if (!animInst->Montage_IsPlaying(AttackMontage)) // 공격 애니메이션이 실행중이지 않은 상태.
	{
		if (true == animInst->GetbAttackEnded()) // recovery 상태 혹은 아무 상태가 아님 --> 공격 유효
		{
			animInst->Montage_Play(AttackMontage, AttackMontagePlayRate);
			animInst->SetbAttackEnded(false);
			// 매번 다른 애니메이션이 실행될 수 있도록.
			animInst->Montage_JumpToSection(FName(*FString::Printf(TEXT("PoleAttack%d"), AttackComboNum)), AttackMontage);
			if (AttackComboNum >= 3)
				AttackComboNum = 0;
			else
				AttackComboNum += 1;
		}
		// else -> 아무 상태도 아니에요, 들어올 경우가 없는 상태로 예상. (recovery 상태가 아니에요.) --> 공격은 씹혀요.
	}
	else // 공격 애니메이션이 실행중일 때, == 공격 키를 연타해서 함수에 들어온 상태임.
	{
		if (true == animInst->GetbAttackEnded()) // 지금 현재 애니메이션이 recovery 상태 --> 공격 유효 !
		{
			//TODO 애니메이션 실행 속도가 너무 빨라지니 애니메이션이 씹혀요. 주의해서 값 조절.
			animInst->Montage_SetPlayRate(AttackMontage, AttackMontagePlayRate * 1.3f); // Recovery 애니메이션 속도를 빠르게 해서 얼른 애니메이션이 끝나도록. 
			animInst->Montage_Play(AttackMontage, AttackMontagePlayRate);
			animInst->SetbAttackEnded(false);
			// 매번 다른 애니메이션이 실행될 수 있도록.
			animInst->Montage_JumpToSection(FName(*FString::Printf(TEXT("PoleAttack%d"), AttackComboNum)), AttackMontage);
			if (AttackComboNum >= 3)
				AttackComboNum = 0;
			else
				AttackComboNum += 1;
		}
		// else // false == animInst->GetbAttackEnded() // 지금 현재 공격 상태 (recovery 상태는 아직 오지 않아따..) --> 공격은 씹혀요.
	}
}

void AMainPlayer::OnMontageEndedCallback(UAnimMontage* Montage, bool bInterrupted)
{
	//! AttackMontage의 현재 몽타주 섹션이 끝났을 때,
	//! 공격중인 상태 = false;
	bIsAttacking = false;
}