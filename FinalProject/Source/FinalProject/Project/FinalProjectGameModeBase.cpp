// Copyright Epic Games, Inc. All Rights Reserved.

#include "FinalProjectGameModeBase.h"

#include "../object/MainPlayer.h"
#include "Blueprint/UserWidget.h"
#include "../UI/MainPlayerUserWidget.h"

AFinalProjectGameModeBase::AFinalProjectGameModeBase()
{
	DefaultPawnClass = AMainPlayer::StaticClass();

    // UserWidgetClass를 블루프린트 경로에서 로드
    static ConstructorHelpers::FClassFinder<UMainPlayerUserWidget> WidgetClassFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/BP_MainPlayerUI.BP_MainPlayerUI_C'")); // 블루프린트 경로
    if (WidgetClassFinder.Succeeded())
    {
        MainPlayerHPWidgetClass = WidgetClassFinder.Class; // UserWidgetClass에 설정
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("FAIL : MainPlayerHPWidgetClass is not Load."));
    }
}

void AFinalProjectGameModeBase::BeginPlay()
{
    if (MainPlayerHPWidgetClass)  // 블루프린트 UI 클래스가 설정되어 있는지 확인
    {
        // UI 인스턴스 생성
        MainPlayerHPWidgetInst = CreateWidget<UMainPlayerUserWidget>(GetWorld(), MainPlayerHPWidgetClass);
        if (MainPlayerHPWidgetInst)
        {
            // UI를 화면에 추가
            MainPlayerHPWidgetInst->AddToViewport();
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("FAIL : MainPlayerHPWidgetClass is not Set."));
    }
}