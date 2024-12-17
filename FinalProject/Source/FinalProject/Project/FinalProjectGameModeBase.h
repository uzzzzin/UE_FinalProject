// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FinalProjectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FINALPROJECT_API AFinalProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UMainPlayerUserWidget> MainPlayerHPWidgetClass; // ���� Ŭ���� ����

private:
	UPROPERTY()
	class UMainPlayerUserWidget* MainPlayerHPWidgetInst; // ������ UI �ν��Ͻ�����.

public:
	AFinalProjectGameModeBase();
};
