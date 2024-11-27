// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerAnimInstance.h"

UMainPlayerAnimInstance::UMainPlayerAnimInstance()
{
}

void UMainPlayerAnimInstance::SetMouseYaw(float _v)
{
	if (0 != _v)
	{
		InputMouseYaw += _v;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("Mouse Yaw : %f"), InputMouseYaw));
	}
}

void UMainPlayerAnimInstance::SetMousePitch(float _v)
{
	if (0 != _v)
	{
		InputMousePitch += _v;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("Mouse Pitch : %f"), InputMousePitch));
	}
}