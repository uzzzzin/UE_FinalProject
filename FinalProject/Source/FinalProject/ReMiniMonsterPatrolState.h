// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State/BaseState.h"
#include "ReMiniMonsterPatrolState.generated.h"

/**
 * 
 */
UCLASS()
class FINALPROJECT_API UReMiniMonsterPatrolState : public UBaseState
{
	GENERATED_BODY()
	
private:
	FTimerHandle repeatTimer; // �ݺ��ؼ� ���� Ÿ�̸�
	float speed; // ���� �ӵ�?
	float rotSpeed; // ȸ�� �ӵ�

public:
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;

public:
	UReMiniMonsterPatrolState();
};
