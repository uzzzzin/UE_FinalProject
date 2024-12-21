// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State/BaseState.h"
#include "ReMiniMonsterTraceState.generated.h"

/**
 * 
 */
UCLASS()
class FINALPROJECT_API UReMiniMonsterTraceState : public UBaseState
{
	GENERATED_BODY()
	
public:
	class AMainPlayer* target; // �Ѿư� Ÿ��.
	float speed; // ���� �ӵ�?
	float rotSpeed; // ȸ�� �ӵ�

public:
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;

public:
	UReMiniMonsterTraceState();
};
