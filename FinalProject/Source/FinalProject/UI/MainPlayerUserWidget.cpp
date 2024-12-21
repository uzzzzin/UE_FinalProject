// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerUserWidget.h"
#include "Components/Image.h"
#include "../Object/MainPlayer.h"

void UMainPlayerUserWidget::HighlightQSkillImage()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "HighlightQSkillImage()");
    if (Q_img)
    {
        Q_img->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.4f)); // ��ų �̹����� ���İ��� �����߾��.
    }
}

void UMainPlayerUserWidget::HighlightESkillImage()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "HighlightESkillImage()");
    if (E_img)
    {
        E_img->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.4f)); // ��ų �̹����� ���İ��� �����߾��.
    }
}

void UMainPlayerUserWidget::RecoveryQSkillImage()
{
    if (Q_img)
    {
        Q_img->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f)); // ���İ��� �ٽ� �ǵ�����.
    }
}

void UMainPlayerUserWidget::RecoveryESkillImage()
{
    if (E_img)
    {
        // �̹����� ��� ����� ���� �귯�� ƾƮ ���� ����
        E_img->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f)); // ���İ��� �ٽ� �ǵ�����.
    }
}
