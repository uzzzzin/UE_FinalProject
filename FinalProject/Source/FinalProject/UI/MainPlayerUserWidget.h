// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainPlayerUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALPROJECT_API UMainPlayerUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UImage* Q_img;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,meta = (BindWidget))
	class UImage* E_img;

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HighlightQSkillImage();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HighlightESkillImage();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void RecoveryQSkillImage();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void RecoveryESkillImage();
};
