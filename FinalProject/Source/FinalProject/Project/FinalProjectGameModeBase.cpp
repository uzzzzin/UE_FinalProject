// Copyright Epic Games, Inc. All Rights Reserved.

#include "FinalProjectGameModeBase.h"

#include "../object/MainPlayer.h"

AFinalProjectGameModeBase::AFinalProjectGameModeBase()
{
	DefaultPawnClass = AMainPlayer::StaticClass();
}
