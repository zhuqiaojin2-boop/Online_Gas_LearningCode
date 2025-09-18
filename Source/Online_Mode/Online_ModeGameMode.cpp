// Copyright Epic Games, Inc. All Rights Reserved.

#include "Online_ModeGameMode.h"
#include "Online_ModeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOnline_ModeGameMode::AOnline_ModeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
