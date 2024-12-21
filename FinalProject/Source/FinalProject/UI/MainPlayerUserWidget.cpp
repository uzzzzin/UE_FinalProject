// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerUserWidget.h"
#include "Components/Image.h"
#include "../Object/MainPlayer.h"

void UMainPlayerUserWidget::HighlightQSkillImage()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "HighlightQSkillImage()");
    if (Q_img)
    {
        Q_img->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.4f)); // 스킬 이미지의 알파값을 조절했어요.
    }
}

void UMainPlayerUserWidget::HighlightESkillImage()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "HighlightESkillImage()");
    if (E_img)
    {
        E_img->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.4f)); // 스킬 이미지의 알파값을 조절했어요.
    }
}

void UMainPlayerUserWidget::RecoveryQSkillImage()
{
    if (Q_img)
    {
        Q_img->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f)); // 알파값을 다시 되돌려요.
    }
}

void UMainPlayerUserWidget::RecoveryESkillImage()
{
    if (E_img)
    {
        // 이미지를 밝게 만들기 위해 브러시 틴트 색상 변경
        E_img->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f)); // 알파값을 다시 되돌려요.
    }
}
