// Copyright Epic Games, Inc. All Rights Reserved.

#include "FinalProjectGameModeBase.h"

#include "../object/MainPlayer.h"
#include "Blueprint/UserWidget.h"
#include "../UI/MainPlayerUserWidget.h"

AFinalProjectGameModeBase::AFinalProjectGameModeBase()
{
	DefaultPawnClass = AMainPlayer::StaticClass();

    // UserWidgetClass�� �������Ʈ ��ο��� �ε�
    static ConstructorHelpers::FClassFinder<UMainPlayerUserWidget> WidgetClassFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_MainPlayerUI.BP_MainPlayerUI_C'")); // �������Ʈ ���
    if (WidgetClassFinder.Succeeded())
    {
        MainPlayerHPWidgetClass = WidgetClassFinder.Class; // UserWidgetClass�� ����
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("FAIL : MainPlayerHPWidgetClass is not Load."));
    }
}

void AFinalProjectGameModeBase::BeginPlay()
{
    if (MainPlayerHPWidgetClass)  // �������Ʈ UI Ŭ������ �����Ǿ� �ִ��� Ȯ��
    {
        // UI �ν��Ͻ� ����
        MainPlayerHPWidgetInst = CreateWidget<UMainPlayerUserWidget>(GetWorld(), MainPlayerHPWidgetClass);
        if (MainPlayerHPWidgetInst)
        {
            // UI�� ȭ�鿡 �߰�
            MainPlayerHPWidgetInst->AddToViewport();
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("FAIL : MainPlayerHPWidgetClass is not Set."));
    }
}