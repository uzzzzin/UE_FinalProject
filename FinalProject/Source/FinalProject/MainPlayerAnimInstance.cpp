// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerAnimInstance.h"
#include "MainPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

UMainPlayerAnimInstance::UMainPlayerAnimInstance()
	: bIsMoving(false)
	, bIsAttacking(false)
	, bIsSiuuuuAttacking(false)
	, bIsSiuuuuTiming(false)
	, SiuuuuAttackDuration(1.f)
{
}

void UMainPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMainPlayer* Owner = Cast<AMainPlayer>(TryGetPawnOwner());
	if (Owner)
	{
		//! BP-> Anim State 세팅
		bIsAttacking = Owner->GetIsAttacking();
		bIsMoving = Owner->GetIsMoving();
		bIsJumping = Owner->GetIsJumping();
		bIsSiuuuuAttacking = Owner->GetIsSiuuuuAttacking();

		if (false == bIsMoving) // State : Move -> Idle 초기 상태 변경
		{
			InputMoveHorizontal = 0.f;
			InputMoveVertical = 0.f;
		}
		else // true == bIsMoving // State : Idle -> Move 초기 상태 변경
		{
			InputMouseYaw = 0.f;
			InputMousePitch = 0.f;
		}
	}

	if (true == bIsSiuuuuTiming)
	{
		//TODO: Siuuuu 타이머 돌아가는 동안에만 동작해요. 공격 투사체 와다다다 던질 예정.
	}
}

void UMainPlayerAnimInstance::AddMouseYaw(float _v)
{
	if (0 != _v)
	{
		InputMouseYaw += _v; // Input값을 누적시켜서 캐릭터가 보는 각도 조절.
	}
}

void UMainPlayerAnimInstance::AddMousePitch(float _v)
{
	if (0 != _v)
	{
		InputMousePitch += _v; // Input값을 누적시켜서 캐릭터가 보는 각도 조절.
	}
}

void UMainPlayerAnimInstance::AddMoveHorizontal(float _v)
{
	if (0 == _v)// 좌,우 움직임이 없는 상태라면
	{
		float wantV = FMath::FInterpTo(InputMoveHorizontal, 0.0f, GetWorld()->GetDeltaSeconds(), 5.f); // BS의 값을 0으로 서서히 되돌리기
		InputMoveHorizontal = wantV;
	}
	else
	{
		if (InputMoveHorizontal >= 45.f)
		{
			InputMoveHorizontal = 45.f;
		}
		else if(InputMoveHorizontal <= -45.f)
		{
			InputMoveHorizontal = -45.f;
		}
		else
		{
			InputMoveHorizontal += _v * 2; // 조금 더 빠른 값 누적을 위한
		}
	}
}

void UMainPlayerAnimInstance::AddMoveVertical(float _v)
{
	if (0 == _v) // 앞, 뒤 움직임이 없는 상태라면
	{
		float wantV = FMath::FInterpTo(InputMoveVertical, 0.0f, GetWorld()->GetDeltaSeconds(), 5.f); // BS의 값을 0으로 서서히 되돌리기
		InputMoveVertical = wantV;
	}
	else
	{
		if (InputMoveVertical >= 20.f)
		{
			InputMoveVertical = 20.f;
		}
		else if (InputMoveVertical <= -20.f)
		{
			InputMoveVertical = -20.f;
		}
		else
		{
			InputMoveVertical += _v * 2; // 조금 더 빠른 값 누적을 위한
		}
	}
}
void UMainPlayerAnimInstance::AnimNotify_EndIdleAttack()
{
	AMainPlayer* Owner = Cast<AMainPlayer>(TryGetPawnOwner());
	Owner->SetIsAttacking(false);
	Owner->SetMovementMaxWalkSpeed(Owner->GetDefaultMovementMaxWalkSpeed());
}

void UMainPlayerAnimInstance::AnimNotify_EndMoveAttack()
{
	//! MoveAttack 애니메이션이 끝나니, MoveAttack도 끝난 거예요.
	if (bIsAttacking)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, "UMainPlayerAnimInstance::AnimNotify_EndMoveAttack()");
		//TODO: 중간에 딜레이 걸어서 공격 시 효과 같은 거 넣어야 해요.
		AMainPlayer* Owner = Cast<AMainPlayer>(TryGetPawnOwner());
		Owner->GetCharacterMovement()->MaxWalkSpeed = 0.0f; // 타이머 시간동안 캐릭터가 움직이지 않을 거예요.
		GetWorld()->GetTimerManager().SetTimer(AnimTimer, this, &UMainPlayerAnimInstance::DelayEndMoveAttack, 1.f,false);
	}
}

void UMainPlayerAnimInstance::DelayEndMoveAttack()
{
	//! 타이머가 끝나면 MoveAttack State가 끝남.
	//! 다시 캐릭터가 움직일 수 있도록, 0으로 바꿔둔 Max Walk Speed를 기본값으로 되돌려준다.
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, "UMainPlayerAnimInstance::DelayEndMoveAttack()");
	AMainPlayer* Owner = Cast<AMainPlayer>(TryGetPawnOwner());
	Owner->SetIsAttacking(false);
	Owner->SetMovementMaxWalkSpeed(Owner->GetDefaultMovementMaxWalkSpeed()); 
	GetWorld()->GetTimerManager().ClearTimer(AnimTimer);
}

void UMainPlayerAnimInstance::AnimNotify_StartJump()
{
	if (bIsJumping)
	{
		AMainPlayer* Owner = Cast<AMainPlayer>(TryGetPawnOwner());
		Owner->Jump();
	}
}

void UMainPlayerAnimInstance::AnimNotify_StartSiuuuuAttack()
{
}

void UMainPlayerAnimInstance::AnimNotify_EndSiuuuuAttack()
{
	bIsSiuuuuTiming = true; // 이제 SiuuuuStop으로 넘어가요.
}

void UMainPlayerAnimInstance::AnimNotify_StartSiuuuuStop()
{
	//TODO: Siuuuu Stop Timer 작동시켜야 해요
	AMainPlayer* Owner = Cast<AMainPlayer>(TryGetPawnOwner());
	Owner->GetCharacterMovement()->MaxWalkSpeed = 0.0f; // 타이머 시간동안 캐릭터가 움직이지 않을 거예요.
	GetWorld()->GetTimerManager().SetTimer(AnimTimer, this, &UMainPlayerAnimInstance::AfterTimerSiuuuuStop, SiuuuuAttackDuration, false);
}

void UMainPlayerAnimInstance::AfterTimerSiuuuuStop()
{
	AMainPlayer* Owner = Cast<AMainPlayer>(TryGetPawnOwner());
	Owner->SetMovementMaxWalkSpeed(Owner->GetDefaultMovementMaxWalkSpeed());

	bIsSiuuuuTiming = false; // 타이머 끝났으니까 이제 Siuuuu 타임은 끝났어요.
	Owner->SetIsSiuuuuAttacking(false); // SiuuuuAttack도 끝나요.
	Owner->SetIsAttacking(false); // Siuuuu Attack 이 모두 끝났으니 공격은 False 가 되어요.
	GetWorld()->GetTimerManager().ClearTimer(AnimTimer);

}