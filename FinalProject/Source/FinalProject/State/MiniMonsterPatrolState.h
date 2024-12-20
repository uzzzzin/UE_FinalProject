// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "State.h"
#include "MiniMonsterPatrolState.generated.h"
/**
 * 
 */
UCLASS()
class FINALPROJECT_API UMiniMonsterPatrolState : public UObject, public IState
{
	GENERATED_BODY()
	
private:
	FTimerHandle repeatTimer; // �ݺ��ؼ� ���� Ÿ�̸�
	float speed; // ���� �ӵ�?
	float rotSpeed; // ȸ�� �ӵ�

public:
	virtual void Enter_Implementation() override;
	virtual void Update_Implementation(float DeltaTime) override;
	virtual void Exit_Implementation() override;

public:
	UMiniMonsterPatrolState();
};
